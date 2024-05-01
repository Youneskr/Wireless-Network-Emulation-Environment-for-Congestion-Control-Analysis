/*
    Kernel Congestion Window Size Monitor
    This program monitors the congestion window size (CWND) directly extracted from the Linux kernel,
    using a custom monitoring module (track_metrics.h).
    It continuously reads the kernel log messages and extracts the CWND metric, 
    then writes the time-series data to a file.

    Usage: ./cwnd_monitor <output_directory>
    - <output_directory>: The directory where the output file will be written.

    The program uses popen() to execute the "dmesg --follow" command, which continuously prints kernel log messages.
    It parses each log message to extract the CWND metric and the corresponding timestamp.
    The extracted data is then written to a file in the specified output directory.

    The output file format:
    - Column 1: Elapsed time since the program started (in seconds).
    - Column 2: Congestion window size (in bytes).

    Note: This program assumes that the CWND metric in kernel log messages is represented as "[timestamp] CWND size" 
    as defined by our integrated monitoring module (track_metrics.h).

    The output file path is constructed using the output directory provided as a command line argument.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CWND 1
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    // Check command line arguments
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <output_directory>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Open pipe to execute dmesg command
    FILE *fp = popen("dmesg --follow", "r");
    if (!fp) {
        perror("popen() error");
        return EXIT_FAILURE;
    }

    // Initialize variables
    char buffer[BUFFER_SIZE];
    double initial_time = 0;
    double current_time;
    int metric;
    long val;

    // Open output file for writing
    char output_path[256];
    snprintf(output_path, 256, "%s/H1-H2-KERNEL-CWND.dat", argv[1]);
    FILE *cwnd = fopen(output_path, "w");
    if (!cwnd) {
        perror("fopen() error");
        pclose(fp);
        return EXIT_FAILURE;
    }

    // Read kernel log messages and extract CWND metric
    while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        sscanf(buffer, "[%*f] %d", &metric);
        if (metric == CWND) {
            sscanf(buffer, "[%lf]", &current_time);
            sscanf(buffer, "[%*f] %*d %ld", &val);
            if (initial_time == 0) {
                initial_time = current_time;
            }
            fprintf(cwnd, "%lf\t%ld\n", current_time - initial_time, val);
            fflush(cwnd);
        }
    }

    // Close files and pipe
    fclose(cwnd);
    pclose(fp);

    return 0;
}
