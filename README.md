# brainfuck-assembler
A system that allows you to transform assembler code to brainfuck.

How it works:
===
You write a program using assembler for my virtual machine (it's called Brainfuck VM or BVM,
see its specification [here](assembler/brainfuck_assembler_specification_4.txt)). Then,
using a special tool, you can embed your program's bytecode into BVM realization written on brainfuck.
Finally, you can run your favourite interpreter or use my [bf2c](codegen/bf_to_c.cpp) translator.
*Attention!* Canonical brainfuck by Urban Müller uses 30K-cell memory tape. I don't know how exactly long
should it be to run my BVM, but 2M cells is good enough :)


Usage:
===
If you have troubles with some of these steps, look through this [Makefile](basm/Makefile).

1. Write your program on my assembler
2. Compile it using [compiler](assembler/compiler.py)
3. Link parts of your program using [linker](assembler/linker.py)
4. Combine your bytecode with BVM using [interpreter](codegen/interpreter.cpp) (Huh, it's not really an interpreter, but I call it so for some irrational reasons)
5. You've got ~1G brainfuck source code. Do anything you want with it!
