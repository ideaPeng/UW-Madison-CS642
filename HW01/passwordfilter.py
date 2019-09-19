#!/usr/bin/env python3
import fileinput
import re

UPPER_CASE = "QWERTYUIOPASDFGHJKLZXCVBNM"
LOWER_CASE = "qwertyuiopasdfghjklzxcvbnm"
SYMBOL = r"~`!@#$%^&*()+=_-{}[]\|:;”’?/<>,."
DIGIT = "0123456789"

ALL_CHARS = (UPPER_CASE, LOWER_CASE, SYMBOL, DIGIT)


def main():
    for line in fileinput.input():
        type_list = [0, 0, 0, 0]
        flag_invalid = False
        for char in line:
            flag_in = False
            for idx, charset in enumerate(ALL_CHARS):
                if char in charset:
                    type_list[idx] = 1
                    flag_in = True
                    break
            if not flag_in and char != '\n':
                flag_invalid = True
                break
        if flag_invalid or len(line) < 6:
            continue
        if sum(type_list) > 2:
            print(line, end="")


if __name__ == "__main__":
    main()
