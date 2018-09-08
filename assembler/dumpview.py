#!/usr/bin/python3

import pickle
import sys

fname = sys.argv[1]

with open(fname, "rb") as f:
    obj = pickle.load(f)

print(obj)
