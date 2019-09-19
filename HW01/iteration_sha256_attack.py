#!/usr/bin/env python3
import fileinput
import hashlib
import sys


def get_hash(input_pass_str):
    h = hashlib.sha256(input_pass_str.encode("ascii")).digest()
    for _ in range(254):
        h = hashlib.sha256(h).digest()
    return hashlib.sha256(h).hexdigest()

def main():
    print(get_hash("ironman,password,84829348943"))
    for idx, line in enumerate(fileinput.input()):
        passwd = line[:-1]
        sys.stdout.write("Download progress: %.2f%%  Trying: %s \r" % (idx/2455868, passwd) )
        sys.stdout.flush()
        result = get_hash("bucky,{},8934029034".format(passwd))
        if result == "f8e2ab8b1678cc51b68f866e4ef7c477c7ba33a731fce412de60c2f8f5e21c11":
            print("\nSuccess! password: %s" % passwd)
            break


if __name__ == "__main__":
    main()
