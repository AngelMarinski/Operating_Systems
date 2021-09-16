#!/bin/bash

input=$(while read line
do
    if ( echo "$line" | grep -q "^[-]\{0,1\}[[:digit:]]\+$" );then
        echo "$line"
    fi
done)

max_sum=-1
number=0

while read line
do
    sum=$(echo "$line" | sed "s/-//g" | grep -o "." | awk 'BEGIN{total=0}{total+=$1}END{print total}')
    if [ $sum -gt $max_sum ];then
        max_sum=$sum
        number=$line
    fi
done < <(echo "$input" | sort -n)

echo "Output: $number"