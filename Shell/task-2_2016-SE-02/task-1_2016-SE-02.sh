#!/bin/bash

if [ ! -f $1 ];then
    echo "Wrong argument passed!"
    exit 1
fi

output=$(cat -n $1 | sed 's/^[[:space:]]\+//g' | sed 's/[[:space:]]\+/ /g' | sed 's/^\([[:digit:]]\+\)/\1\./g')
echo "$output" | sed 's/^\([[:digit:]]\+\. \)\([[:digit:]]\+ [[:alpha:]]\. - \)\([[:print:]]\+\)/\1\3/g' | sort -t ' ' -k2