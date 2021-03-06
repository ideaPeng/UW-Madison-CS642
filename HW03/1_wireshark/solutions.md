# Your UW ID and your name - there is no special format for this. (This homework ought to be done individually)
fang67
Fang, Yidong

# Trace 1: HTTP
## Give three websites (domain Nname and IP addresses) visited from source IP address `192.168.0.100`
```
1) www.amazon.com 205.251.242.54
2) www.baidu.com 180.76.3.151
3) www.bing.com 198.105.251.25
```

## Give three search queries and the domain of the site for each query made from source IP address `192.168.0.100`
```
1) completion.amazon.com
/search/complete?method=completion&q=ad&search-alias=stripbooks&client=amazon-search-ui&mkt=1&fb=1&xcat=0&x=updateISSCompletion&sc=1&noCacheIE=1393230034198
2) www.baidu.com
/s?wd=%E6%9D%A5%E8%87%AA%E6%98%9F%E6%98%9F%E7%9A%84%E4%BD%A0&rsv_bp=0&ch=&tn=baidu&bar=&rsv_spt=3&ie=utf-8&rsv_sug3=14&rsv_sug4=794&rsv_sug1=3&rsv_sug2=0&inputT=2
3) www.bing.com 198.105.251.25
/search?q=chicago+metro&go=%E6%8F%90%E4%BA%A4&qs=n&form=QBRE&pq=chicago+metro&sc=8-13&sp=-1&sk=&cvid=3dafaeb94bb04c35a9efa4621371beb1
```

# Trace 2: FTP
## What is the user name and password used to connect to the FTP server?
```
USER shiningmoon
PASSWORD public
```

## List any (and all) files that were downloaded.
```
/dragon.zip
/phase1/ARP.java
/phase1/L2Switch.java
/TeNet/phase1.html
```

## List the full path for two files (in different directories) on the FTP server that were NOT downloaded.
```
/jerrygen.zip
/phase1/StoreForwardingDatalink.java
```

# Trace 3: Traceroute
## Briefly describe how the traceroute tool works including which network protocols are in use.
```
The tool will send UDP messages with increasing Time-To-Live (TTL) of IP protocol from 1 to receive the TTL exceeded ICMP packets from the hosts on the path between the source IP and destination IP, which complain the TTL is used up or exceeded. Thus, the tool will know the IP address of the devices between itself and the target host. 
```

## Give the source IP address that issued the traceroute command and the destination IP address.
```
source IP address: 192.168.0.100
destination IP address: 74.125.225.46
```

## List the IP addresses on the route between source and destination.
```
source IP address: 192.168.0.100
192.168.0.1
10.131.180.1
96.34.20.20
96.34.17.95
96.34.16.112
96.34.16.77
96.34.2.4
96.34.0.7
96.34.0.9
96.34.3.9
96.34.152.30
209.85.254.120
209.85.250.28
destination IP address: 74.125.225.46
```

# Trace 4: POP
## What is the POP username and password?
```
username: cs155@dummymail.com
password: whitehat
```

## How many emails are in the user's mailbox?
```
There are five emails.
```

## Give the contents of from, to, subject, and date for one email message.
```
From: joe <cs155@dummymail.com>
To: cs155@dummymail.com
Subject: foobar
Date: Fri, 23 Apr 2010 08:20:52 -0700
```

## What email client (application) and operating system is this person using to send and receive email?

```
email client: Thunderbird 2.0.0.23 
operating system: Windows/20090812
```
