#!/bin/bash

output_exec="main"
env=$1

# Find all .c files recursively in current directory and subdirectories
source_files=$(find . -type f -name '*.c')
depedencies="-pthread -ljson-c -lpq -I/usr/include/postgresql"

gcc -o "$output_exec" $depedencies ${source_files} -D"$env"

if [ $? -eq 0 ]; then
    ./"$output_exec"
else
    echo "Compilation failed."
fi