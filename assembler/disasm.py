#!/usr/bin/python3

import sys
import os

def init_opcodes():
    localdir = os.path.dirname(os.path.realpath(__file__))
    with open(localdir + os.path.sep + "instructions.txt") as fin:
        lines = fin.readlines()
    opcodes = list()
    for idx, line in enumerate(lines):
        name, args = line.split()
        opcodes.append(name)
    return opcodes

def main():
    opcodes = init_opcodes()
    for fname in sys.argv[1:]:
        foutname = os.path.splitext(fname)[0] + os.path.extsep + 'bdisasm'
        with open(fname, 'r') as fin, open(foutname, 'w') as fout:
            codes = fin.read().split()
            for code in codes: 
                print(opcodes[int(code, 16)] if len(code) == 2 else code, file=fout)


if __name__ == '__main__':
    main()
