#!/bin/bash

gcc -o db -ljson-c db.c

if ! [ $? -eq 0 ]; then
    echo "C program failed with an error"
else
    ./db
fi

