#!/bin/bash

if [ ! -d  $1 ];then
    echo "Wrong arguments passed!"
    exit 1
fi

if [ $# -eq 1 ];then
    broken=$(find -L $1 -type l)

    find $1 -type l | grep -v "$(find -L $1 -type l)" | xargs -I {} stat {} -c "%N"

    echo "Broken symlinks: $(echo "$broken" | wc -l)"

elif [ $# -eq 2 ];then
    broken=$(find -L $1 -type l)

    find $1 -type l | grep -v "$(find -L $1 -type l)" | xargs -I {} stat {} -c "%N">>/$2

    echo "Broken symlinks: $(echo "$broken" | wc -l)" >>./$2
else
    echo "Wrong arguments passed!"
    exit 2
fi

exit 0