#!/bin/bash

if [ $# -eq 2 ] && (echo "$1 $2" | grep -q "^[[:digit:]]\+ [[:digit:]]\+$") && [ $2 -gt $1 ];then
    mkdir ./a ./b ./c
    while read line
    do
        if [ $(cat $line | wc -l) -le $1 ];then
            mv $line ./a
        elif [ $(cat $line | wc -l) -ge $1 ] && [ $(cat $line | wc -l) -lt $2 ];then
            mv $line ./b
        else
            mv $line ./c
        fi
    done < <(find . -type f)

    exit 0
else
    echo "Wrong arguments passsed!"
    exit 1
fi