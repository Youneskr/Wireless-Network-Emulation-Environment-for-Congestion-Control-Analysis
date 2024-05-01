#!/bin/bash

# This script calculates the average of numbers in a file.

# Usage: ./calculate_average.sh <file_name>
# - Replace <file_name> with the name of the file containing numbers.
# - The file should contain numbers separated by whitespace.

# Example:
#   ./calculate_average.sh RESULTS/cubic/H1-H2-KERNEL-CWND.dat
#   This command will calculate the average of CWND size in the file "RESULTS/cubic/H1-H2-KERNEL-CWND.dat".
#

calculate_average() {
    local file_name=$1
    local average=$(awk '{sum += $2} END {print sum/NR}' "$file_name")
    echo "$average"
}