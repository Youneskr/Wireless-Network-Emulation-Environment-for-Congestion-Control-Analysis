## Network Simulation Environment

This repository contains scripts and configurations for setting up a network emulation environment using Mininet-WiFi. The environment facilitates testing and analysis of various network scenarios, including congestion control, interference, and latency.

## Project Structure
The project consists of the following files and directories:

<pre>
.
├── client.c
├── cwnd_monitor.c
├── track_metrics.h
├── README.md
├── Mininet
│   ├── config.sh
│   ├── interference.py
│   └── latency.py
└── scripts
    ├── calculate_average.sh
    ├── find_latest_subdirectory.sh
    ├── generate_fct_summary.sh
    └── metric_average.sh
</pre>

## Scripts Overview

# Root Directory

- **`client.c`:** C program representing a TCP client socket with congestion control. It sends a specified amount of random data to a server and measures various metrics such as Round Trip Time (RTT) and Sending Rate.

- **`cwnd_monitor.c`:** C program for monitoring the Congestion Window Size (CWND) directly extracted from the Linux kernel. It continuously reads kernel log messages and writes time-series data to a file.

- **`track_metrics.h`:** Header file defining functions to monitor various network metrics within the Linux kernel. It provides capabilities to track standard metrics such as CWND, BDP, and bandwidth, alongside BBR-specific parameters.


# Mininet Directory

- **`interference.py`:** Python script for setting up a network topology with interference. It supports scenarios with varying levels of interference.

- **`latency.py`:** Python script for configuring a network topology with different end-to-end propagation delays.

- **`config.py`:** Sets up background traffic using iperf between servers and clients.

# Scripts Directory

- **`generate_fct_summary.sh`:** Bash script for generating a summary of flow completion times (FCT) for different congestion control schemes in a given scenario.

- **`metric_average.sh`:** Bash script for generating the average value of a specific metric across different congestion control schemes for a given scenario.

- **`find_latest_subdirectory.sh`:** Bash script for finding the latest subdirectory for a given congestion control scheme and scenario.

- **`calculate_average.sh`:** Bash script for calculating the average of numbers in a file.

## Usage

1. Choose the appropriate script or program based on the network scenario you want to simulate.
2. Modify any parameters or configurations within the scripts if necessary.
3. Execute the script or program in your environment.

Each script and program has its own usage instructions, which are documented in the file itself. Please refer to the individual files for detailed usage instructions.

## Contributing
Contributions are welcome.