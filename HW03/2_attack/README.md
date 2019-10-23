# CS 642 Intro to Information Security - HW 03

This homework is supposed to practice our skills in the Network security area.

A scanner of pcap file which acts as a tiny IDS.

## Description

- ARP Attack Detection: To detect the ARP message that with same MAC addresses but different IPv4 address(except `0.0.0.0`).
- Port Scan Detection: To detect TCP-SYN and UDP packets that sends to same IPv4 address but at more than 100 ports.
- TCP SYN Floods Detection: To detect SYN packets that same to the same IPv4 address with speed exceeding 100 packets per second and without corresponding ACK packets.

## Dependency

- Python3
- dpkt==1.9.2

## How to run
```shell
python3 scanner.py <file-to-detect.pcap>
```

