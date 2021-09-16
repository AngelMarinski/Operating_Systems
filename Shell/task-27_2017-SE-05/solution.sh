#!/bin/bash

if [ $# -ne 2 ] || [ ! -d $1 ];then
    echo "Wrong arguments passed!"
    exit 1
fi

find $1 -maxdepth 1 -type f | grep "$2" | sort -nr -t '.' -k123 | head -1