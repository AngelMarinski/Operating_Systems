#!/bin/bash

while read user home
do
    echo "$user $(find $home -type f -printf "%P %C@\n" 2>~/err.container | sort -nr -t ' ' -k3 | head -1)"
done < <(cat /etc/passwd | cut -d ':' -f1,6 | tr ':' ' ') | sort -nr -t ' ' -k3 | cut -d ' ' -f1,2 | head -1

exit 0