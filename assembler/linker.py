#!/usr/bin/python3

import pickle
import sys
import os
from version import *
import argparse

class Object:
    def __init__(self, obj, offset, fname):
        self.offset = offset

        self.defined_local_symbols = set()
        self.defined_global_symbols = set()

        for symbol in obj["defined_symbols"]:
            if symbol[0].startswith("!"):
                self.defined_local_symbols.add(symbol)
            else:
                self.defined_global_symbols.add(symbol)

        self.required_local_symbols = set()
        self.required_global_symbols = set()
        for symbol in obj["required_symbols"]:
            if symbol[0].startswith("!"):
                self.required_local_symbols.add(symbol)
            else:
                self.required_global_symbols.add(symbol)

        self.code = obj["code"]
        self.version = obj.setdefault("version", "N/A")
        self.fname = fname

    def process_local_symbols(self, allocator):
        symbols = dict()
        for (name, value, symbol_type) in self.defined_local_symbols:
            if name in symbols.keys():
                raise Exception("Redefinition of local symbol " + name + " in " + self.fname)
            else:
                if symbol_type == "label":
                    symbols[name] = value + self.offset
                elif symbol_type == "var":
                    symbols[name] = allocator.malloc(value)
                else:
                    raise Exception("Unknown symbol type: " + name + " in " + self.fname)

        for (name, offset) in self.required_local_symbols:
            if name in symbols.keys():
                insert_int(self.code, symbols[name], offset)
            else:
                raise Exception("Undefined local symbol " + name + " in " + self.fname)


def insert_int(arr, n, pos):
    arr[pos:pos + 2] = (n % 2 ** 16).to_bytes(2, 'little')

def load_obj(fname, offset):
    with open(fname, "rb") as f:
        obj = pickle.load(f)
    return Object(obj, offset, fname)

class StaticAllocator:
    def __init__(self, size):
        self.free_ = 0
        self.size_ = size

    def malloc(self, size):
        ans = self.free_
        self.free_ += size
        if self.free_ > self.size_:
            raise Exception("Not enough memory")
        return ans

    def size(self):
        return self.free_


def main():
    localdir = os.path.dirname(os.path.realpath(__file__))
    argparser = argparse.ArgumentParser(description='Compile brainfuck assembler files to BOBJ format')
    argparser.add_argument("-o", "--output", default='a.bexe')
    argparser.add_argument("-b", "--boot-code", default=localdir + os.path.sep + "init.bobj")
    argparser.add_argument("input", nargs="*")
    argparser.add_argument("-V", "--version", action="version", version=VERSION)
    args = argparser.parse_args(sys.argv[1:])

    objects = []
    fnames = [args.boot_code] + args.input
    # offset = 0
    codeAlloc = StaticAllocator(CODE_BANK_SIZE)
    dataAlloc = StaticAllocator(DATA_BANK_SIZE)

    for fname in fnames:
        objects.append(load_obj(fname, codeAlloc.size()))
        codeAlloc.malloc(len(objects[-1].code))

        if objects[-1].version != VERSION:
            raise Exception("Object file \"" + objects[-1].fname + "\" version is not supported: " + objects[-1].version + " found, " + VERSION + " expected")

    global_symbols = dict()

    global_symbols["__allocated__"] = 0 # will be redefined after processing all symbols
    global_symbols["NULL"] = dataAlloc.malloc(2) # null pointer

    for obj in objects:
        obj.process_local_symbols(dataAlloc)

    for obj in objects:
        for (name, value, symbol_type) in obj.defined_global_symbols:
            if name in global_symbols.keys():
                raise Exception("Redefinition of global symbol " + name + " in " + obj.fname)
            else:
                if symbol_type == "label":
                    global_symbols[name] = value + obj.offset
                elif symbol_type == "var":
                    global_symbols[name] = dataAlloc.malloc(value)
                else:
                    raise Exception("Unknown symbol type: " + name + " in " + obj.fname)

    global_symbols["__allocated__"] = dataAlloc.size()

    for obj in objects:
        for (name, value) in obj.required_global_symbols:
            if name in global_symbols.keys():
                insert_int(obj.code, global_symbols[name], value)
            else:
                raise Exception("Undefined global symbol " + name + " in " + obj.fname)


    with open(args.output, "wb") as f:
        for obj in objects:
            f.write(bytearray(obj.code))

if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        print("Error: " + str(e))
