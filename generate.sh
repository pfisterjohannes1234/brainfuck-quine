#!/bin/bash

#Script to generates the brainfuck code#

set -eu

scriptError()
{
  echo "Error on line $1" 1>&2
  exit 1
}

trap 'scriptError $LINENO' ERR

mkdir temporary/ || true

#Generate a program to generate brainfuckcode to generate asciicode (stored in cells and not printed)
gcc -Wall -Wextra -Wpedantic ./ascii-brainfuck.c -o ./temporary/ascii-brainfuck


#Generate a "simpler" version of template_.c, so that we can convert it to brainfuck code
gcc -DGENERATE_SIMPLE=1 -E template.c | grep -v '^#' > ./temporary/template_.c

#Format the code. Not needed but can it make easier to debug the resulting brainfuck code in
# case we use a brainfuck interpreter that can output debug information including line number
#Since it it optional, we ignore any error (including command not found)
astyle --style=gnu --indent=spaces=2 ./temporary/template_.c -n || true

#Create the brainfuck code of the printing part
python3 ./convert.py < ./temporary/template_.c > ./temporary/printcode.bf

#Remove all unnecessary characters. Note: tr doesn't accept every order
# but '][+,.><-' seems to work
#This can help performance.
tr -cd '][+,.><-'  < ./temporary/printcode.bf > ./temporary/printcode_minimal.bf

#Create the brainfuck code of the generateasciicode part
./temporary/ascii-brainfuck -n 4 < ./temporary/printcode_minimal.bf > ./temporary/generatecode.bf

cat ./temporary/generatecode.bf ./temporary/printcode_minimal.bf > quine.bf



