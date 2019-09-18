#!/usr/bin/env python2
# CS 642 University of Wisconsin
#
# WARNING:
# Do not use this encryption functionality, it has security vulnerabilities!
#
# Your job is to find and understand the problems
#
# usage: badencrypt.py keyfile
#

import sys
import os
import Crypto.Cipher.AES
import hashlib

f = open(sys.argv[1], 'r')
key = f.readline()
key = key[:32].decode("hex")
f.close()

message = \
"""AMOUNT: $  10.00
Originating Acct Holder: Hugh
Orgininating Acct #82123-09837

I authorized the above amount to be transferred to the account #38108-443280 
held by a Wisc student at the National Bank of the Cayman Islands.
"""

iv = os.urandom(16)
cipher = Crypto.Cipher.AES.new(key, Crypto.Cipher.AES.MODE_CBC, IV=iv)
ciphertext = cipher.encrypt(message).encode("hex")
tag = hashlib.sha256(message).hexdigest()
# print iv.encode("hex")
# print ciphertext
# print tag
print iv.encode("hex") + ciphertext + tag
