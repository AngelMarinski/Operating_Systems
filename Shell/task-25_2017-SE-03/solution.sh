#!/bin/bash

if [ "$(whoami)" != "s62469" ];then
    echo "You have no permission to complete this action!"
    exit 1
fi


while read line
do
    user=$(echo "$line" | awk '{print $2}')
    rss=$(ps --user "$user" -o rss= | awk 'BEGIN{total=0}{total+=$1}END{print total}')
    echo "user: $user processes: $(echo "$line" | awk '{print $1}') total rss: $rss"
done < <(ps -e -o user= | sort | uniq -c)

while read line
do
    avg_memory=$(ps --user $line -o rss= | awk 'BEGIN{total=0}{total+=$1}END{print total/NR*2}')
    flag=$(ps --user $line -o rss= --sort=-rss | head -1 | awk -v avg=$avg_memory '{if($1 > avg){print 1}else{print 0}}')

    if [ $flag -eq 1 ];then
        #kill -KILL $(ps --user $line -o pid=,rss= --sort=-rss | head -1 | awk '{print $1}')
        echo "KILL pid: $(ps --user $line -o pid=,rss= --sort=-rss | head -1 | awk '{print $1}')  of user: $line"
    fi
done < <(ps -e -o user= | sort | uniq)

exit 0