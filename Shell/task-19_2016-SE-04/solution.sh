#!/bin/bash

if [ $# -ne 2 ] || [ ! -f $1 ] || [ ! -f $2 ] ;then
    echo "Wrong arguments passed!"
    exit 1
fi

if [ $(grep "$1" $1 | wc -l) -gt $(grep "$2" $2 | wc -l) ];then
    cat $1 | grep -o "\"[[:print:]]\+\"[[:print:]]\+$" | sort -d >> ./$1.songs
elif [ $(grep "$1" $1 | wc -l) -lt $(grep "$2" $2 | wc -l) ];then
     cat $2 | grep -o "\"[[:print:]]\+\"[[:print:]]\+$" | sort -d >> ./$1.songs
else
    echo "Files has equal lines matching their filename"
fi

exit 0