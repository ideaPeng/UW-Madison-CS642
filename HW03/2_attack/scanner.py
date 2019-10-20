#!/usr/bin/env python3
'''
A simple IDS
some code are borrowed from the examples of dpkt
'''
import argparse
import socket
import datetime

try:
    import dpkt
except ImportError as error:
    print("Necessary packages not installed! Please install it!")
    exit(1)

from dpkt.compat import compat_ord

def mac_addr(address):
    """Convert a MAC address to a readable/printable string

       Args:
           address (str): a MAC address in hex form (e.g. '\x01\x02\x03\x04\x05\x06')
       Returns:
           str: Printable/readable MAC address
    """
    return ':'.join('%02x' % compat_ord(b) for b in address)

def inet_to_str(inet):
    """Convert inet object to a string

        Args:
            inet (inet struct): inet network address
        Returns:
            str: Printable/readable IP address
    """
    # First try ipv4 and then ipv6
    try:
        return socket.inet_ntop(socket.AF_INET, inet)
    except ValueError:
        return socket.inet_ntop(socket.AF_INET6, inet)

def detect_arpspoofing(data):
    '''
    Simple version: only check sha and spa
    '''
    # only check arp packet
    target_eth_type = dpkt.ethernet.ETH_TYPE_ARP
    arp_records = dict()
    attacker = set()
    for idx, (time_stamp, pkt_data) in enumerate(data):
        pkt_data = dpkt.ethernet.Ethernet(pkt_data)
        if pkt_data.type == target_eth_type :
            sha = pkt_data.arp.sha
            spa = pkt_data.arp.spa
            if sha not in arp_records.keys():
                arp_records[sha] = set()
                arp_records[sha].add(spa)
            else:
                arp_records[sha].add(spa)
                if len(arp_records[sha]) > 1:
                    print("ARP spoofing!\nMAC: {}\nPacket number: {}".format(mac_addr(sha), idx+1))
                    return True

def detect_port_scan(data):
    target_eth_type = dpkt.ethernet.ETH_TYPE_IP
    ports_records = dict()
    for idx, (time_stamp, pkt_data) in enumerate(data):
        pkt_data = dpkt.ethernet.Ethernet(pkt_data)
        if pkt_data.type != target_eth_type :
            continue
        # only IP data allowed
        ip_data = pkt_data.data
        if ip_data.p == dpkt.ip.IP_PROTO_TCP:
            if ip_data.data.flags != dpkt.tcp.TH_SYN:
                continue
            dport = ip_data.data.dport
        elif ip_data.p == dpkt.ip.IP_PROTO_UDP:
            dport = ip_data.data.dport
        else:
            continue
        # TCP or UDP scanning
        if ip_data.dst not in ports_records.keys():
            ports_records[ip_data.dst] = (set([dport]), [str(idx+1)])
        else:
            con_set = ports_records[ip_data.dst]
            if dport not in con_set[0]:
                con_set[0].add(dport)
                con_set[1].append(str(idx + 1))
                if (len(con_set[0]) > 100):
                    print("Port scan!\nIP: {}\nPacket number: {}".format(inet_to_str(ip_data.dst), ", ".join(con_set[1])))
                    return True


def detect_syn_floods(data):
    target_eth_type = dpkt.ethernet.ETH_TYPE_IP
    syn_pkts = dict()
    flag = False
    # first filter out the SYN data
    for idx, (time_stamp, pkt_data) in enumerate(data):
        pkt_data = dpkt.ethernet.Ethernet(pkt_data)
        if pkt_data.type != target_eth_type :
            continue
        ip_data = pkt_data.data
        if ip_data.p != dpkt.ip.IP_PROTO_TCP:
            continue
        if ip_data.data.flags == dpkt.tcp.TH_SYN:
            new_rec = (str(idx+1), time_stamp, pkt_data)
            if ip_data.dst not in syn_pkts.keys():
                syn_pkts[ip_data.dst] = [new_rec]
            else:
                syn_pkts[ip_data.dst].append(new_rec)
    # check for each dst ip
    for key, val in syn_pkts.items():
        if len(val) < 100:
            continue
        i = 0
        j = 99
        while j<len(val):
            if val[j][1]-val[i][1] <= 1:
                pkt_num_list = [x for (x, _, _) in val[i:j+1]]
                flag = True
                print("SYN floods!\nIP: {}\nPacket number: {}".format(inet_to_str(key), ", ".join(pkt_num_list)))
                break
        if flag:
            break
    return flag


def main():
    parser = argparse.ArgumentParser(description="A simple intrusion detection system to detect potential attack or dangerous behaviour in network activity.")
    parser.add_argument("pcap_file", type=argparse.FileType("rb"))
    args = parser.parse_args()
    pcap = dpkt.pcap.Reader(args.pcap_file)
    data = list(pcap)
    if detect_port_scan(data):
        return
    if detect_syn_floods(data):
        return
    if detect_arpspoofing(data):
        return

if __name__ == "__main__":
    main()
