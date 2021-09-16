#!/bin/bash

if [ $# -ne 3 ] || [ ! -d $1 ] || [ ! -d $2 ];then
    echo "Wrong arguments passed!"
    exit 1
fi

if [ "$(whoami)" = "root" ];then
    find $1 | grep "$3" | xargs -I {} mv {} $2
else
    echo "You have no permission to complete this action!"
    exit 1
fi