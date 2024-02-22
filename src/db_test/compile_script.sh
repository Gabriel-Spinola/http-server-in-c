#!/bin/bash

gcc db.c -o db

if ! [ $? -eq 0 ]; then
    echo "C program failed with an error"
else
    ./db
fi

