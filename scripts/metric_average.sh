#!/bin/bash


# This script generates the average value of a specific metric across different congestion control schemes for a given scenario.
#
# Usage: ./metric_average.sh <scenario> <metric>
# - Replace <scenario> with the scenario number.
# - Replace <metric> with the desired metric.
# - Ensure that the necessary sub-scripts, find_latest_subdirectory.sh and calculate_average.sh, are present in the "scripts" directory.
#
# Example:
#   ./metric_average.sh 1 [CWND|SENDING-RATE|RTT]
#   This command will generate the average value of the specified metric (CWND, SENDING-RATE, or RTT) across different congestion control schemes for scenario 1.
#
# Note: This script assumes the presence of specific directory structures and file naming conventions.

# Check if correct number of arguments are provided
if [ "$#" -ne 2 ]; then
    echo "Error: Incorrect number of arguments provided."
    echo "Usage: ./metric_average.sh <scenario> <metric>"
    exit 1
fi

# Get the scenario value and metric
scenario="$1"
metric="$2"

source ./scripts/find_latest_subdirectory.sh
source ./scripts/calculate_average.sh

# Define the protocols array
protocols=("bbr" "bbr2" "cubic" "vegas" "reno")

# Define the files array
declare -a files

# Loop through each protocol
for protocol in "${protocols[@]}"; do
    subdir=$(find_latest_subdirectory "$protocol" "$scenario")
    files+=("$(printf "%s/H1-H2-KERNEL-%s.dat" "$subdir" "$metric")")
done

# Define the output file path
output="RESULTS/SC$scenario/${metric^^}-AVERAGE.dat"

# Open the output file for writing
exec 3>"$output"

# Loop through each protocol file and write the data to the output file
for ((i=0; i<${#protocols[@]}; i++)); do
    # Convert protocol names
    case "${protocols[i]}" in
        "bbr") protocol_name="BBRv1" ;;
        "bbr2") protocol_name="BBRv2" ;;
        "cubic") protocol_name="CUBIC" ;;
        "vegas") protocol_name="Vegas" ;;
        "reno") protocol_name="Reno" ;;
    esac

    # Read data from the file
    average=$(calculate_average "${files[i]}")
    # Write protocol name and data to the output file
    echo -e "$protocol_name\t$average" >&3
done

# Close the output file
exec 3>&-
