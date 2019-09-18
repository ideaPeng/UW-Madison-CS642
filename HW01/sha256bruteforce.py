#!/usr/bin/env python3
import hashlib


def main():
    print(hashlib.sha256("hugh,13145820,20193833".encode("ascii")).hexdigest())
    # 13145820
    guess_flag = True
    digits = 1
    while guess_flag:
        bound = 10**digits
        guess = 0
        while guess < bound:
            guess_str = ("hugh,{:0" + str(digits) +
                         "d},20193833").format(guess)
            print(guess_str, end='\r')
            result = hashlib.sha256(guess_str.encode("ascii")).hexdigest()
            if result == "ee688ca24c201a27fcc94ebd46e87ae6a7c4f54b445fccfc0727a70332353f7f":
                print("Right! %s" % guess)
                guess_flag = False
                break
            guess += 1
        digits += 1

if __name__ == "__main__":
    main()
