#!/bin/bash

if [ "$(whoami)" != "root" ];then
    echo "You have no permission to complete this action!"
    exit 1
fi

while read user home
do
    if [ ! -d $home ] || [ $(stat $home -c "%U") != $user ] || [ ! -w $home ];then
        root_rss=$(ps --user "root" -o rss= | awk 'BEGIN{total=0}{total+=$1}END{print total}')
        user_rss=$(ps --user "$user" -o rss= | awk 'BEGIN{total=0}{total+=$1}END{print total}')

        if [ $user_rss -gt $root_rss ];then
            #ps --user "$user" -o pid= | xargs -I {} kill -KILL {}
            ps --user $user -o user=,pid=
        fi
    fi
done < <(cat /etc/passwd | grep -v "root" | cut -d ':' -f1,6 | tr ':' ' ')