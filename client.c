/*
    TCP Client with Congestion Control
    This program represents a TCP client socket that sends 120MB of random data to a server using a specified congestion control scheme.
    The congestion control scheme is passed as a command line argument.

    Usage: ./client <congestion_control_scheme> <output_path>
    - <congestion_control_scheme>: The congestion control scheme to be used.
    - <output_path>: The path where the output files will be written.

    The Linux kernel maintains internal data structures to track active TCP connections and their parameters. 
    This program utilizes getsockopt() to retrieve TCP parameters for the current connection using the TCP_INFO option.
    The TCP_INFO structure contains various TCP parameters, including round trip time (tcpi_rtt), which is used to calculate the RTT.

    The program measures and computes the following metrics:
    - Round Trip Time (RTT): Extracted from the TCP_INFO structure using getsockopt().
    - Sending Rate: Calculated by dividing the amount of data sent by the elapsed time.
    - Flow Completion Time: Recorded when the client finishes sending 120MB of data.

    All metrics are written to separate output files in the specified output path.

    Other metrics such as congestion window size, retransmission ratio are computed and extracted separately using other scripts.

    The server IP address is set in the code and can be modified if necessary.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <time.h>

#define PORT 1234
#define CHUNK_SIZE 1024
#define SERVER_IP "10.0.0.2" 
#define DATA_SIZE (120 * 1024 * 1024) // 120MB

// Function to generate random data
void generate_random_data(char *buffer, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        buffer[i] = rand() % 256; // Random byte
    }
}

int main(int argc, char *argv[]) {
    // Check command line arguments
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <congestion_control_scheme> <output_path>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Variables for collecting TCP information
    struct tcp_info tcp_info;
    socklen_t tcp_len = sizeof(tcp_info);

    // Variables for measuring time and computing metrics
    double rtt_ms, sending_rate_Mbps, elapsed_time = 0;
    struct timeval start_time, end_time;

    // Extract command line arguments
    char *congestion_control = argv[1];
    char *path = argv[2];

    // Open files for writing results
    char sending_rate_path[256], rtt_path[256], flow_completion_time_path[256];
    snprintf(sending_rate_path, 256, "%s/H1-H2-SENDING-RATE.dat", path);
    snprintf(rtt_path, 256, "%s/H1-H2-RTT.dat", path);
    snprintf(flow_completion_time_path, 256, "%s/H1-H2-FLOW-COMPLETION-TIME.dat", path);
    FILE *sending_rate = fopen(sending_rate_path, "w");
    FILE *rtt = fopen(rtt_path, "w");
    FILE *flow_completion_time = fopen(flow_completion_time_path, "w");

    // Socket and connection setup
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation error");
        return EXIT_FAILURE;
    }

    // Set TCP congestion control scheme
    if (setsockopt(sock, IPPROTO_TCP, TCP_CONGESTION, congestion_control, strlen(congestion_control) + 1) < 0) {
        perror("Setsockopt TCP_CONGESTION failed");
        return EXIT_FAILURE;
    }

    struct sockaddr_in server_address = {0};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        return EXIT_FAILURE;
    }

    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        return EXIT_FAILURE;
    }

    printf("Connected to server\n");

    // Allocate memory for data message
    char *message = (char *)malloc(DATA_SIZE);
    if (!message) {
        perror("Failed to allocate memory for message");
        return EXIT_FAILURE;
    }
    generate_random_data(message, DATA_SIZE);

    // Start sending data
    ssize_t total_sent = 0;
    time_t start, end = time(NULL);
    gettimeofday(&start_time, NULL);
    fprintf(sending_rate, "0.000000\t0.00\n");
    while (total_sent < DATA_SIZE) {
        ssize_t sent = send(sock, message + total_sent, CHUNK_SIZE, 0);
        if (sent < 0) {
            perror("Send failed");
            return EXIT_FAILURE;
        }

        total_sent += sent;

        // Record flow completion time
        if (total_sent >= DATA_SIZE) {
            fprintf(flow_completion_time, "%.6f", elapsed_time);
        }

        // Calculate elapsed time
        gettimeofday(&end_time, NULL);
        end = time(NULL);
        elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                       (end_time.tv_usec - start_time.tv_usec) / 1000000.0;

        // Get TCP information
        if (getsockopt(sock, SOL_TCP, TCP_INFO, &tcp_info, &tcp_len) < 0) {
            perror("getsockopt(TCP_INFO)");
            exit(EXIT_FAILURE);
        }

        // Extract Round Trip Time (RTT)
        rtt_ms = ((double)tcp_info.tcpi_rtt) / 1000;
        fprintf(rtt, "%.6f\t%.2f\n", elapsed_time, rtt_ms);

        // Compute Sending Rate
        if (difftime(end, start) != 0) {
            sending_rate_Mbps = ((total_sent * 8) / difftime(end, start)) / 1000000;
            fprintf(sending_rate, "%.6f\t%.2f\n", elapsed_time, sending_rate_Mbps);
        }
    }

    printf("%.2f MB sent to server\n", (float)total_sent / (1024 * 1024));

    // Close connections and files, free memory
    close(sock);
    fclose(sending_rate);
    fclose(rtt);
    fclose(flow_completion_time);
    free(message);

    return 0;
}
