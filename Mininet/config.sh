#!/bin/bash

# This script sets up background traffic using iperf between servers and clients.
# It is used specifically for Scenario 2 to increase interferences.

# Define the command for executing mininet-wifi utilities
cmd="sudo ~/mininet-wifi/util/m"

# Setup Background traffic

# Servers on Host h2
$cmd h2 iperf -s -u -p 2222 > /dev/null &
$cmd h2 iperf -s -u -p 2223 > /dev/null &
$cmd h2 iperf -s -u -p 2224 > /dev/null &

# Servers on Host h3
$cmd h3 iperf -s -u -p 3333 > /dev/null &
$cmd h3 iperf -s -u -p 3334 > /dev/null &
$cmd h3 iperf -s -u -p 3335 > /dev/null &

# Servers on Host h4
$cmd h4 iperf -s -u -p 4444 > /dev/null &
$cmd h4 iperf -s -u -p 4445 > /dev/null &
$cmd h4 iperf -s -u -p 4446 > /dev/null &
$cmd h4 iperf -s -u -p 4447 > /dev/null &

# Allow some time for servers to start
sleep 5

# Clients
$cmd h11 iperf -c -u "10.0.0.2" -p "2222" -t 3600 > /dev/null &
$cmd h12 iperf -c -u "10.0.0.2" -p "2223" -t 3600 > /dev/null &
$cmd h13 iperf -c -u "10.0.0.2" -p "2224" -t 3600 > /dev/null &

$cmd h14 iperf -c -u "10.0.0.3" -p "3333" -t 3600 > /dev/null &
$cmd h15 iperf -c -u "10.0.0.3" -p "3334" -t 3600 > /dev/null &
$cmd h16 iperf -c -u "10.0.0.4" -p "3335" -t 3600 > /dev/null &

$cmd h17 iperf -c -u "10.0.0.4" -p "4444" -t 3600 > /dev/null &
$cmd h18 iperf -c -u "10.0.0.4" -p "4445" -t 3600 > /dev/null &
$cmd h19 iperf -c -u "10.0.0.4" -p "4446" -t 3600 > /dev/null &
$cmd h20 iperf -c -u "10.0.0.4" -p "4447" -t 3600 > /dev/null &
