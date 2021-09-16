#!/bin/bash

input=$(while read line
do
    if ( echo "$line" | grep -q "^[-]\{0,1\}[[:digit:]]\+$" );then
        echo "$line"
    fi
done)
echo "$input"
max=$(echo "$input" | sort -nr | head -1)

echo "Output:"
echo "$input" | grep "[\-]\{0,1\}$max" | uniq