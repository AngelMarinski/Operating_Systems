#!/bin/bash

if [ $# -eq 1 ] && ( echo $1 | grep -q "^[[:digit:]]\+$" ) && [ "$(whoami)" = "root" ];then
    while read line
    do
        rss=$(echo "$line" | awk '{print $3}')
        if [ $rss -gt $1 ];then
            username=$(echo "$line" | awk '{print $1}')
            if ( cat ~/log | grep -q "$username" );then
                continue
            fi
            echo "$username" >> ~/log
            pid=$(ps --user $username -o pid=,rss= --sort=-rss | head -1 | awk '{print $1}')
            #uncomment the following line in order to make the task work
            #kill -KILL $pid
            echo "KILL $pid of user: $username with rss: $rss"
        fi
    done < <(ps -e -o user=,pid=,rss= --sort=-rss)
    exit 0
else
    echo "Wrong arguments passed!"
    exit 1
fi