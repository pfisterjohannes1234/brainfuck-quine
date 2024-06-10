## Goal

The goal of this Project is to create a quine in Brainfuck. For fun.

A quine is a program that prints its own source code without reading it.


## Description

Since it isn't that easy to make a Brainfuck Quine in your head, i created tools to help building a
 quine.

First, we have a C Program that prints brainfuck code to generate the values in a array (used to
 print Part A, see later) and after that it prints the values of the array (used to print Part B).
This Program is specially crafted so that `convert.py` can convert it (after preprocessing) to
 Brainfuck code.
With this one part, part B, of the brainfuck program is created.
This is stored in `./temporary/printcode.bf`

The other brainfuck part, part A, creates the data used to print.
It must generate the data used in the part B of the brainfuck program.
We generate part A automatically. For that we feed part B to `ascii-brainfuck` which generates the
 brainfuck programm to fill the data we need to print Part B.
This is stored in `./temporary/generatecode.bf`

After this, both parts are combined and that results in a brainfuck quine.

## How to generate the quine

The bash script `generate.sh` generates the quine.
It will be stored as `./quine.bf`
For that you need this programs installed.

- bash
- python3
- gcc

Optional programs:

- astyle

## Files / Directories


### File requied

- `generate.sh` : Bash script to generate quine from the other files provied
- `template.c`  : C-Program that generates the printing part. Writting in a way that it can be converted to brainfuck.
- `convert.py`  : Python script to generate brainfuck code from C-Like cod
- `ascii-brainfuck.c`: C-Sourcecode for a program that generates Brainfuck code to store ASCII values in cells.

### File/directores created

- `temporary/`:    To store some temporary files
- `quine.bf` :     The quine we generate

