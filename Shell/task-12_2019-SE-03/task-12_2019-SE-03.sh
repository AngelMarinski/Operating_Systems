#!/bin/bash

inode=$(find ~ -printf "%i %C@\n" | sort -nr -t ' ' -k2 | head -1 | cut -d ' ' -f1)

find ~ -inum $inode -printf "%P %d\n" | sort -n -t ' ' -k2 | head -1