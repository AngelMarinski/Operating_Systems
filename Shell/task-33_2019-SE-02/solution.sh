#!/bin/bash

if [ "$1" = "-n" ];then
    for file in "${@:3}"
    do
        cat $file.log | head -n $2 | sed "s/\([[:digit:]]\+-[[:digit:]]\+-[[:digit:]]\+ [[:digit:]]\+:[[:digit:]]\+:[[:digit:]]\+\) \([[:print:]]\+\)/\1 $file \2/g" | sort
    done
else
    for file in $@
    do
        cat $file.log | head -10 | sed "s/\([[:digit:]]\+-[[:digit:]]\+-[[:digit:]]\+ [[:digit:]]\+:[[:digit:]]\+:[[:digit:]]\+\) \([[:print:]]\+\)/\1 $file \2/g" | sort
    done
fi