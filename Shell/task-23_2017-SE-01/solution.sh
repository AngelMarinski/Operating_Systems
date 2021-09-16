#!/bin/bash

if [ ! -d $1 ];then
    echo "Wrong argument passed!"
    exit 1
fi

if [ $# -eq 1 ];then
    find -L $1 -type l
elif [ $# -eq 2 ] && (echo $2 | grep -q "^[[:digit:]]\+$");then
    find $1 -printf "%P %n\n" | awk -v min=$2 '{if(min <= $2){print $0}}'
else
    echo "Wrong number of arguments passed!"
    exit 2
fi

exit 0