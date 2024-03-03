#!/bin/bash
output_exec="main"

# Find all .c files recursively in current directory and subdirectories
source_files=$(find . -type f -name '*.c')
json_lib="json"

gcc -o "$output_exec" -pthread -ljson-c ${source_files}

if [ $? -eq 0 ]; then
    ./"$output_exec"
else
    echo "Compilation failed."
fi