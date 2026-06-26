#!/bin/bash
in=$1
name=$2
echo "static const char* ${name} ="
while IFS= read -r line; do
    escaped=$(echo "$line" | sed -e 's/"/\\"/g' | sed -e 's/"\\","\\\\"/'/g)
    printf "%s\n" "  \"${escaped}\n\""
done < "$in"
echo ";"
