#!/bin/bash

if [ "$(whoami)" = "root" ];then
    while read user home
    do

        if [ ! -d $home ] || sudo -u $user [ ! -w $home ];then
            echo "$user"
        fi

    done < <(cat /etc/passwd | awk -F ':' '{print $1 $6}')
fi