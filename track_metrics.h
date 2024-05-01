/*
    Track Metrics
    This module defines functions to monitor various network metrics within the Linux kernel.
    It provides capabilities to track standard metrics such as CWND, BDP, and bandwidth, alongside BBR-specific parameters.

    The track() function facilitates the monitoring of standard metrics, providing logs with relevant connection details such as source IP, destination IP, and ports.
    Similarly, the track_bbr_phases() function enables the monitoring of BBR-specific phases and parameters.

    Both functions ensure that the monitored metrics correspond to specific connections (defined by source IP, destination IP, and ports) and the employed congestion control scheme.

    Usage: Include this header file in kernel modules to call the track() and track_bbr_phases() functions for metric monitoring in tcp.c and congestion control scheme sources code located in net/ipv4.
    Note: For access across all kernel modules, place this header file in the kernel's include/linux directory.
    This ensures seamless inclusion and utilization of its functions and definitions throughout the kernel.

    Incoming optimization: The upcoming optimization of this module aims to enhance its flexibility by allowing the user to specify and monitor a specific TCP connection dynamically.
    Instead of statically predefined parameters in the code, users will have the ability to pass source and destination IP addresses and ports as options.
    This enhancement is particularly useful for monitoring TCP connections in Mininet WiFi experiments, where typical IP parameters like "10.0.0.1" and "10.0.0.2" are used as source and destination IPs.
*/


#ifndef TRACK_METRICS_H
#define TRACK_METRICS_H

// Define constants for different metrics
#define CWND 1
#define BDP 2
#define BW 3
#define INFLIGHT 4
#define RATE 5
#define RTT 6
#define MIN_RTT 7

// Define constants for BBR phases
#define STARTUP 11
#define DRAIN 22
#define PROBE_BW 33
#define PROBE_RTT 44

#include <linux/types.h>
#include <linux/string.h>
#include <linux/inet.h>

// Function to track various metrics
void track(const char *cc, __be32 saddr, __be32 daddr, unsigned short sport, unsigned short dport, int METRIC, u64 value)
{
    // Check if the source and destination addresses match the expected values
    if (saddr == htonl(0x0a000001) && daddr == htonl(0x0a000002)) {
        // Print the metric and its value
        printk(KERN_INFO "%d\t%llu\t%lu\n", METRIC, value, hash_cnx);
    }
}

// Function to track BBR phases
void track_bbr_phases(const char *cc, __be32 saddr, __be32 daddr, unsigned short sport, unsigned short dport, int PHASE, u32 PACING_GAIN, u32 CWND_GAIN)
{
    // Check if the source and destination addresses match the expected values and the congestion control is BBR or BBR2
    if (saddr == htonl(0x0a000001) && daddr == htonl(0x0a000002) && (strcmp(cc, "BBR") == 0 || strcmp(cc, "BBR2") == 0) ) {
        // Print the phase and its gains
        printk(KERN_INFO "%d\t%u\t%u\n", PHASE, PACING_GAIN, CWND_GAIN);
    }
}

#endif
