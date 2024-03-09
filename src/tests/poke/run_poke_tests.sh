#!/bin/bash
for test_file in *.json; do
    if [[ -f "$test_file" ]]; then
        echo "SENDING REQUEST: $test_file"
        poke net send -f "$test_file"
    fi
done