Part1 (40 pts)

In this part, you will find four packet traces (pcap files) that can  be read by the Wireshark tool (among other tools). You will need to  investigate these traces to answer the questions below. To get started  you will want to understand how to use Wireshark's filtering  capabilities. Your solution will be a file `solutions.txt` with answers to the questions below.

### Trace 1: HTTP (10 pts)

1. Give three websites (domain name and IP addresses) visited from source IP address 192.168.0.100.
2. Give three search queries and the domain of the site for each query made from source IP address 192.168.0.100.

### Trace 2: FTP (10 pts)

FTP is the file transport protocol. There is a lot of information about it on the internet.

1. What is the username and password used to connect to the FTP server?
2. List any (and all) files that were downloaded from the FTP server.
3. List the full path for two files (in different directories) on the FTP server that were NOT downloaded.

### Trace 3: Traceroute (10pts)

Traceroute is a tool used to determine the route between two IP addresses. You can find information about it on the internet. 

1. Briefly describe how the traceroute tool works including which network protocols are in use.
2. Give the source IP address that issued the traceroute command and the destination IP address.
3. List the IP addresses on the route between source and destination.

### Trace 4: POP (10 pts)

The post-office protocol (POP) is used for email.

1. What is the POP username and password?
2. How many emails are in the user’s mailbox?
3. Give the contents of from, to, subject, and date for one email message.
4. What email client (application) and operating system is this person using to send and receive email?

 

## Part 2 (60 pts)

In this part, you will write a simple intrusion detection system to  detect potential attacks or dangerous behavior in network activity.

Here are three pcaps with example attacks in folder 2: 

1.  `arpspoofing.pcap` includes an ARP spoof attack. IP address 192.168.0.100 advertises the wrong MAC address for 192.168.0.1.
2.  `portscan.pcap` includes a TCP SYN port scan.
3.  `tcpflood.pcap` includes a TCP SYN flood.

Your job is to write a software IDS (a Python script named **scanner****.py**) that takes as input a pcap trace and looks for such malicious behavior. The local network you are protecting is configured with two machines  (192.168.0.100 with MAC address 7c:d1:c3:94:9e:b8 and 192.168.0.103 with MAC address d8:96:95:01:a5:c9) and a router (192.168.0.1 with MAC  address f8:1a:67:cd:57:6e). Your scanner should:

\1. Detect ARP spoofing attempts. (20 pts)

Output a warning including the offending MAC address and the packet  number of the offending packet. The format of your output should be:

```
     ARP spoofing!
     MAC: XX:XX:XX:XX:XX:XX
     Packet number: XX
```

\2. Detect port scans. (20 pts)

A port scan is defined to occur whenever TCP SYNs or UDP packets are  sent to a 100 or more different ports on a target system. The scanner  should output a warning including the victim destination IP address, and the offending packet numbers. The format of your output should be:

```
     Port scan!
     IP: XX.XX.XX.XX
     Packet number: XX, XX, XX, XX
```

\3. Detect TCP SYN floods. (20 pts)

Your tool should detect when the number of TCP SYNs to a particular  destination (that are not associated with completed handshakes) exceeds  100 per second. The scanner should output a warning including the victim destination IP address, and the offending packet numbers. The format of your output should be:

```
     SYN floods!
     IP: XX.XX.XX.XX
     Packet number: XX, XX, XX, XX
```

Your program should take as input the filename of a pcap file that contains captured network packets, for example:

```
 python scanner.py example.pcap
```

The output of your program will be the warning messages as described above. Please also write a **README** to explain *how to run your code* and *give one line of description of each kind of your scanners*. Check that your scanner runs properly on the CS lab machines before turning it in.

We will test your program on new pcap files other than the three we provide.

### *Resources:*

We recommend using **dpkt** and **scapy** library for reading pcap files and scanning through different packet  headers. Follow the instructions mentioned in the documentation to get  the packages installed in your machines.

 

## Deliverables:

Submit 3 separate files: 

1. **solution.txt** wrt the1st part, 
2.  **scanner.py** wrt the 2nd part,
3.  **Readme.md** explaining *how to run your code and giving one line of description of each kind of your scanners.* 