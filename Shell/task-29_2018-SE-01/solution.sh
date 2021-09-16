#!/bin/bash

if [ $# -ne 1 ] || [ ! -d $1 ];then
    echo "Wrong argument passed!"
    exit 1
fi

while read friend
do
    lines=$(find $1 -type f | grep "$friend" | xargs -I {} cat {} | wc -l)
    echo "$friend $lines"
done < <(find $1 -maxdepth 3 -mindepth 3 | sed "s/[[:print:]]\+\/\([[:print:]]\+\)/\1/g" | sort | uniq) | sort -nr -t ' ' -k2 | head -10