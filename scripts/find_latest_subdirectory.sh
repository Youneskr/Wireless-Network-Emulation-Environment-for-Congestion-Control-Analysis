#!/bin/bash

# Usage: find_latest_subdirectory.sh <congestion_control> <scenario>

# This script finds the latest subdirectory for a given congestion control scheme and scenario.

# Arguments:
#   - <congestion_control>: The congestion control scheme.
#   - <scenario>: The scenario number.

# Example usage:
#   latest_subdir=$(find_latest_subdirectory.sh cubic 1)

find_latest_subdirectory() {
    local congestion_control="$1"
    local scenario="$2"
    local dir_path="RESULTS/SC$scenario/$congestion_control"
    local latest_dir=""
    local last_mod=0  # Initialize last_mod variable

    # Check if the directory exists
    if [ ! -d "$dir_path" ]; then
        echo "Error: Unable to open directory $dir_path"
        return 1
    fi

    # Loop through all subdirectories
    for subdir in "$dir_path"/*/; do
        # Get the modification time of the subdirectory
        local mtime=$(stat -c %Y "$subdir")
        
        # Check if this subdirectory was modified more recently than the previous ones
        if [ "$mtime" -gt "$last_mod" ]; then
            last_mod="$mtime"
            latest_dir=$(basename "$subdir")
        fi
    done

    # Check if any subdirectory was found
    if [ -z "$latest_dir" ]; then
        echo "Error: No subdirectories found in $dir_path"
        return 1
    fi

    # Output the latest subdirectory path
    echo "RESULTS/SC$scenario/$congestion_control/$latest_dir"
}

# Uncomment the following lines to use the script directly with command-line arguments
# scenario=$1
# congestion_control=$2
# latest_subdir=$(find_latest_subdirectory "$congestion_control" "$scenario")
