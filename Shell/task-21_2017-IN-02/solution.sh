#!/bin/bash

if [ $# -ne 1 ] || [ $(whoami) != "root" ];then
    echo "Wrong argument passed!"
    exit 1
fi

processes=$(ps --user $1 | wc -l)

while read line
do
    if [ $(echo "$line" | awk '{print $1}') -gt $processes ];then
        echo "$line" | awk '{print $2}'
    fi
done < <(ps -e -o user= | sort | uniq -c)

avg_time=$(ps -e -o times= | awk 'BEGIN{total=0}{total=total+$1}END{print (total/NR)*2}')
echo $avg_time
while read line
do
    flag=$(echo "$line" | awk -v avg=$avg_time '{if(avg<$3)}{print 1}')
    if [ $flag -eq 1 ];then
        #kill -KILL $(echo "$line" | awk '{print $1}')
        echo "KILL process of user: $(echo "$line" | awk '{print $1}') pid: $(echo "$line" | awk '{print $2}')"
    fi
done < <(ps --user $1 -o user=,pid=,times=)

exit 0