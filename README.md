# brainfuck-assembler
It is a system that allows you to transform assembler code into brainfuck code.

How it works:
===
You write a program using assembler for my virtual machine (it's called Brainfuck VM or BVM,
see its specification [here](assembler/brainfuck_assembler_specification_4.txt)). Then,
using a special tool, you can embed your program's bytecode into BVM realization written in brainfuck.
Finally, you can run your favourite interpreter or use my [bf2c](codegen/bf_to_c.cpp) translator.
**Attention!** The canonical brainfuck by Urban Müller uses 30K-cell memory tape. I don't know how long 
exactly it should be to run my BVM, but 2M cells are good enough :)


Usage:
===
If you have troubles with some of these steps, look through this [Makefile](basm/Makefile).

1. Write your program in my assembler
2. Compile it using [compiler](assembler/compiler.py)
3. Link parts of your program using [linker](assembler/linker.py)
4. Combine your bytecode with BVM using [interpreter](codegen/interpreter.cpp) (Huh, it's not really an interpreter, but I call it so for some irrational reasons)
5. Now you've got ~1Gb brainfuck source code. Do anything you want with it!

Immoral programming
===
Some time ago I tried to write an article about mechanisms used in this project. Unfortunately, I couldn't pull it off. If you're interested in my project and familiar with Russian, you're welcome to finish [this paper](immoral_programming/immoral_programming.pdf))
