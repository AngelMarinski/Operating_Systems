#!/bin/bash

if [ -d $1 ];then
    echo "Symlinks: "
    find $1 -type l
    echo "Broken symlinks: "
    find -L $1 -type l
fi

exit 0