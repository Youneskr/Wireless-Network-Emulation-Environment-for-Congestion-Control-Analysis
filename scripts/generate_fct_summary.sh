#!/bin/bash


# This script generates a summary of flow completion times (FCT) for different congestion control schemes in a given scenario.
#
# Usage: ./generate_fct_summary.sh <scenario>
# - Replace <scenario> with the scenario number.
# - Ensure that the necessary sub-script, find_latest_subdirectory.sh, is present in the "scripts" directory.
#
# Example:
#   ./generate_fct_summary.sh 1
#   This command will generate a summary of FCT for scenario 1 across different congestion control schemes.
#
# Note: This script assumes the presence of specific directory structures and file naming conventions.

# Check if correct number of arguments are provided
if [ "$#" -ne 1 ]; then
    echo "Error: Incorrect number of arguments provided."
    echo "Usage: ./generate_fct_summary.sh <scenario>"
    exit 1
fi

source ./scripts/find_latest_subdirectory.sh

# Define the protocols array
protocols=("bbr" "bbr2" "cubic" "vegas" "reno")

# Define the files array
declare -a files

# Get the scenario value
scenario="$1"

# Loop through each protocol
for protocol in "${protocols[@]}"; do
    subdir=$(find_latest_subdirectory "$protocol" "$scenario")
    files+=("$(printf "%s/H1-H2-FLOW-COMPLETION-TIME.dat" "$subdir")")
done

# Define the output file path
output="RESULTS/SC$scenario/FCT-SUMMARY.dat"

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
    read -r data < "${files[i]}"

    # Write protocol name and data to the output file
    echo -e "$protocol_name\t$data" >&3
done

# Close the output file
exec 3>&-
