#!/usr/bin/python
# coding=utf-8

"""
This script establishes a network topology using Mininet-WiFi, facilitating network simulations for testing and analysis. 
The configuration of the network is determined by a scenario number passed as a command-line argument. 
The script sets up a wireless access point (ap1), 9 switches, a wireless station (h1), and 3 hosts (h2, h3, h4).
Additional hosts are added in Scenario 2.

The valid scenario numbers are [0, 1, 2], each representing a unique network setup:
- Scenario 0 (SC0): Sets up a network without any interference.
- Scenario 1 (SC1): Introduces interference into the network with specific parameters: 
                    noise threshold of -91 and a fading coefficient of 3.
- Scenario 2 (SC2): Extends Scenario 1 by adding 10 additional wireless nodes to the network, 
                    simulating a higher level of interference and providing a more challenging 
                    environment for testing and analysis.

Usage:
    python interference.py -sc <scenario_number>

Example:
    python interference.py -sc 1

This command will set up a network according to Scenario 1, introducing interference with predefined parameters.
"""

import argparse, os
from mininet.node import Controller
from mininet.node import RemoteController
from mininet.log import setLogLevel, info
from mn_wifi.cli import CLI
from mn_wifi.net import Mininet_wifi
from mn_wifi.link import wmediumd
from mn_wifi.wmediumdConnector import interference

def topology(scenario):
    # Different network configurations based on the scenario number
    if scenario == 0:
        net = Mininet_wifi(controller=Controller, link=wmediumd)
    elif scenario == 1 or scenario == 2:
        net = Mininet_wifi(controller=Controller, link=wmediumd, wmediumd_mode=interference, noise_th=-91, fading_cof=3)

    c0 = net.addController('c0', controller=RemoteController, ip='127.0.0.1',port=6653)
    
    info("*** Creating nodes\n")
    ap1 = net.addAccessPoint('ap1', ssid="ap1", mac='00:00:00:00:00:01', mode="g", channel="5", position='500,1250,0', range=200)

    net.addStation('h1', ip='10.0.0.1/8', mac='00:00:00:00:01:01',
                   min_x=400, max_x=600, min_y=1220, max_y=1280, min_v=10, max_v=10, range=100)
    
    if scenario == 2:
        # Add more station  
        i = 11
        while i <= 25:
            net.addStation('h'+str(i), 
                        ip=('10.0.0.' + str(i)),
                        mac='00:00:00:00:01:' + str(i),
                        min_x=358, max_x=600, min_y=1140, max_y=1400, min_v=5, max_v=15, range=100)
            i += 1

    info("*** Configuring Propagation Model\n")
    net.setPropagationModel(model="logDistance", exp=4)

    # Add hosts
    h2 = net.addHost("h2",ip='10.0.0.2/8', mac='00:00:00:00:01:02')
    h3 = net.addHost("h3",ip='10.0.0.3/8', mac='00:00:00:00:01:03')
    h4 = net.addHost("h4",ip='10.0.0.4/8', mac='00:00:00:00:01:04')

    # Add switches
    s2 = net.addSwitch("s2", mac='00:00:00:00:00:02')
    s3 = net.addSwitch("s3", mac='00:00:00:00:00:03')
    s4 = net.addSwitch("s4", mac='00:00:00:00:00:04')
    s5 = net.addSwitch("s5", mac='00:00:00:00:00:05')
    s6 = net.addSwitch("s6", mac='00:00:00:00:00:06')
    s7 = net.addSwitch("s7", mac='00:00:00:00:00:07')
    s8 = net.addSwitch("s8", mac='00:00:00:00:00:08')
    s9 = net.addSwitch("s9", mac='00:00:00:00:00:09')

    info("*** Configuring wifi nodes\n")
    net.configureWifiNodes()

    info( '*** Creating links\n' )
    # Add links between switches
    net.addLink(s2, ap1,bw=80, delay='25ms')
    net.addLink(s2, s9, bw=70, delay='15ms')
    net.addLink(s9, s3, bw=50, delay='20ms')
    net.addLink(s3, s4, bw=50, delay='5ms')
    net.addLink(h2, s4, bw=50, delay='20ms')
    net.addLink(s4, s7, bw=50, delay='5ms')
    net.addLink(s7, s5, bw=70, delay='10ms')
    net.addLink(s5, h3, bw=50, delay='5ms')
    net.addLink(s4, s8, bw=50, delay='5ms')
    net.addLink(s8, s6, bw=60, delay='5ms')
    net.addLink(s6, h4, bw=60, delay='12ms')
    net.addLink(s5, s9, bw=50, delay='10ms')
    net.addLink(s9, s6, bw=50, delay='5ms')
    
    # Set the mobility model
    net.setMobilityModel(time=0, model='RandomDirection', seed=10)
    
    info("*** Starting network\n")
    net.build()

    c0.start()
    ap1.start([c0])
    s2.start([c0])
    s3.start([c0])
    s4.start([c0])
    s5.start([c0])
    s6.start([c0])
    s7.start([c0])
    s8.start([c0])
    s9.start([c0])

    # Run a script if scenario is 2
    if scenario == 2:
        os.system("sudo ./config.sh")

    # Start the CLI
    CLI(net)

    info("*** Stopping network\n")
    # Stop the network
    net.stop()

if __name__ == '__main__':
    setLogLevel( 'info' )
    # Clear Mininet
    os.system('sudo mn -c')

    # Parse command line arguments
    parser = argparse.ArgumentParser(description="Scenario number selection")
    parser.add_argument("-sc", type=int, choices=[0, 1, 2], required=True, help="Scenarios number")
    args = parser.parse_args()

    topology(args.sc)
