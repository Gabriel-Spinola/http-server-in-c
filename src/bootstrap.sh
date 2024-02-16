#!/bin/bash
output_exec="main"

# Find all .c files recursively in current directory and subdirectories
source_files=$(find . -type f -name '*.c')

gcc -o "$output_exec" -pthread ${source_files}

if [ $? -eq 0 ]; then
    ./"$output_exec"
else
    echo "Compilation failed."
fi