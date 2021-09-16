#!/bin/bash

directories=$(find /home/students -maxdepth 1 -name "s*" -printf "%P %C@\n" | awk -F ' ' '{if ( $2>=1621168000 && $2<=1627676101){print $0}}' | cut -d ' ' -f1)

while read line
do
    fn=$(cat /etc/passwd | grep "$line" | cut -d ':' -f1 | cut -c 2-)
    name=$(cat /etc/passwd | grep "$line" | grep -o "[[:alpha:]]\+ [[:alpha:]]\+")
    echo "$fn   $name"
done < <(echo "$directories")


#2nd way to do it:
#cat /etc/passwd | grep --color "^$(find /home/students -name "s*" -maxdepth 1 -type d -newerct '@1621168000' -not -newerct '@1623176101' | cut -d '/' -f4 | grep "^s[[:digit:]]\+")" | cut -d ':' -f1,5 | cut -d ',' -f1 | tr ':' '\t'