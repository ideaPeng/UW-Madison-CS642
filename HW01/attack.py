#!/usr/bin/env python2
# CS 642 University of Wisconsin
#
# usage: attack.py ciphertext
# Outputs a modified ciphertext and tag

import sys
import hashlib

# Grab ciphertext from first argument
ciphertextWithTag = (sys.argv[1]).decode("hex")

if len(ciphertextWithTag) < 16+16+32:
  print("Ciphertext is too short!")
  sys.exit(0)

iv = ciphertextWithTag[:16]
ciphertext = ciphertextWithTag[:len(ciphertextWithTag)-32]
tag = ciphertextWithTag[len(ciphertextWithTag)-32:]

# Modify the input so the transfer amount is more lucrative to the recipient

new_message = \
"""AMOUNT: $9910.00
Originating Acct Holder: Hugh
Orgininating Acct #82123-09837

I authorized the above amount to be transferred to the account #38108-443280 
held by a Wisc student at the National Bank of the Cayman Islands.
"""
# at byte 9,10 change iv to make \x20 to \x39 xor with \x19
# iv[9] xor msg_en = \x20(space) => iv[9] xor \x19 xor msg_en = \x20 xor \x19 (9)
# new iv to change the first block space in to number 9
two_spaces_encrypted = iv[9:11]
new_encrypted = ""
for ch in two_spaces_encrypted:
    new_encrypted += chr(ord(ch) ^ 0x19)
new_iv = iv[:9] + new_encrypted + iv[11:]
new_tag = hashlib.sha256(new_message).hexdigest()
ciphertext = new_iv + ciphertext[16:len(ciphertextWithTag)-32]

# TODO: Print the new encrypted message
print(ciphertext.encode("hex") + new_tag)
