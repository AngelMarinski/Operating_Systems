#!/bin/bash

if [ ! -f $1 ];then
    echo "Wrong argument passed!"
    exit 1
fi

mostfailures=$(cat $1 | tail +2 | grep "Failure" | cut -d '|' -f2 | uniq -c | sort -nr -t ' ' -k1 | awk -F ' ' '{print $2}')
									 
cat $1 | tail +2 | grep "$mostfailures" | sort -nr -t '|' -k1 | head -1 |  sed 's/[[:digit:]]\+|\([[:alpha:]]\+\)|[[:alpha:]]\+|\([[:alpha:]]\+\)/\1:\2/g'

exit 0