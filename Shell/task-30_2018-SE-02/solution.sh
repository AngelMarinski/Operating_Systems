#!/bin/bash

if [ $# -ne 2 ] || [ ! -f $1 ] || [ ! -d $2 ];then
    echo "Wrong arguments passed!"
    exit 1
fi

touch ./$2/dist.txt

cat $1 | sed "s/^\([[:alpha:]]\+ [[:alpha:]]\+\)[[:print:]]\+/\1/g" | sort | uniq | awk 'BEGIN{key=0}{print $1" "$2";"key ; key++}' >> ./$2/dist.txt

while read line
do
    filename=$(echo $line | cut -d ';' -f2)
    touch ./$2/$filename.txt
    cat $1 | grep "$(echo $line | cut -d ';' -f1)" >> ./$2/$filename.txt
done < <(cat ./$2/dist.txt)