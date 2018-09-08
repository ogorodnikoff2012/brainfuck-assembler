#!/usr/bin/python3

import sys
import os.path
import shlex
import argparse

from queue import Queue
import ast
import traceback
import pickle
from version import VERSION

def add_int(arr, n):
    arr += (n % 2 ** 16).to_bytes(2, 'little')

def add_opcode(arr, op):
    arr.append(op)

def init_opcodes():
    localdir = os.path.dirname(os.path.realpath(__file__))
    with open(localdir + os.path.sep + "instructions.txt") as fin:
        lines = fin.readlines()
    opcodes = dict()
    for idx, line in enumerate(lines):
        name, args = line.split()
        opcodes[name.lower()] = (idx, int(args))
    return opcodes

def main():
    opcodes = init_opcodes()
    argparser = argparse.ArgumentParser(description='Compile brainfuck assembler files to BOBJ format')
    argparser.add_argument("-o", "--output-dir", default='.')
    argparser.add_argument("input", nargs="*")
    argparser.add_argument("-V", "--version", action="version", version=VERSION)
    args = argparser.parse_args(sys.argv[1:])

    for finname in args.input:
        try:
            print("Assembling", finname)
            foutname = os.path.join(args.output_dir, os.path.splitext(os.path.basename(finname))[0] + os.path.extsep + 'bobj')

            defined_symbols = set()
            # defined_variables = set()
            required_symbols = set()
            defines = dict()

            code = []
            tokenQueue = Queue()

            with open(finname, "r") as fin:
                tokenList = shlex.split(fin.read(), comments=True, posix=False)
                for t in tokenList:
                    tokenQueue.put_nowait(t)

            while not tokenQueue.empty():
                token = tokenQueue.get_nowait().lower()
                if token == "label":
                    lblName = tokenQueue.get_nowait()[:-1]
                    lblPos = len(code)
                    defined_symbols.add((lblName, lblPos, "label"))
                elif token == "const":
                    constName = tokenQueue.get_nowait() #.lower()
                    constVal = tokenQueue.get_nowait()
                    defines[constName] = constVal
                elif token == "var":
                    varName = tokenQueue.get_nowait()
                    varSize = tokenQueue.get_nowait()

                    if varSize[0] == '$':
                        constName = varSize[1:]
                        if constName not in defines.keys():
                            raise Exception("Unknown const " + constName + " in file " + finname)
                        varSize = defines[constName]

                    varSize = ast.literal_eval(varSize)
                    if type(varSize) is int:
                        defined_symbols.add((varName, varSize, "var"))
                    else:
                        raise Exception("Invalid variable size " + varSize + " in file " + finname)
                elif token == "loadstr":
                    token = tokenQueue.get_nowait()
                    if token[0] == '$':
                        constName = token[1:]
                        if constName not in defines.keys():
                            raise Exception("Unknown const " + constName + " in file " + finname)
                        token = defines[constName]
                    arg = ast.literal_eval(token)
                    if type(arg) is str:
                        prog = ['push', 'comp_a', 'push', 'comp_b', 'push', 'assign_b_const', 2]
                        for ch in arg:
                            prog += ['assign_mem_const', ord(ch), 'assign_a_dp', 'add', 'assign_dp_a']
                        prog += ['assign_mem_const', 0, 'assign_a_dp', 'add', 'assign_dp_a',
                                'pop', 'decomp_b', 'pop', 'decomp_a', 'pop']
                        for t in prog:
                            if type(t) is str:
                                add_opcode(code, opcodes[t][0])
                            else:
                                add_int(code, t)
                    else:
                        raise Exception("Expected string, found " + token + " in file " + finname)
                elif token in opcodes.keys():
                    opcode, args = opcodes[token]
                    add_opcode(code, opcode)
                    for i in range(args):
                        token = tokenQueue.get_nowait()
                        if token[0] == '$':
                            constName = token[1:]
                            if constName not in defines.keys():
                                raise Exception("Unknown const " + constName + " in file " + finname)
                            token = defines[constName]
                        try:
                            arg = ast.literal_eval(token)
                            if type(arg) is str:
                                for ch in arg:
                                    add_int(code, ord(ch))
                            elif type(arg) is int:
                                add_int(code, arg)
                            else:
                                raise Exception
                        except:
                            arg = token
                            lblPos = len(code)
                            required_symbols.add((arg, lblPos))
                            add_int(code, 0)
                else:
                    raise Exception("Unknown token: " + str(token))

            obj = {'defined_symbols': defined_symbols, 'required_symbols': required_symbols, 'code': code, 'version': VERSION}
            with open(foutname, "wb") as fout:
                pickle.dump(obj, fout)

        except:
            traceback.print_exc()

if __name__ == '__main__':
    main()
