Ans:#!/bin/bash

if [ ! -f $1 ];then
    echo "Wrong argument passed!"
    exit 1
fi

type=$( cat $1 | tail -n +2 | sort -nr -t ';' -k3 | head -1 | cut -d ';' -f2 )

cat $1 | grep ";$type;" | sort -n -t ';' -k3 | head -1 | cut -d ';' -f1,4 | sed 's/;/\t/g'