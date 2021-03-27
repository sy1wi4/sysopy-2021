#!/bin/bash
filecount=0
while [ $filecount -lt $1 ] ; do
    filesize=$2
    filesize=$(($filesize+1024))
    base64 /dev/urandom | 
    head -c "$filesize" > $3${filecount}.txt
    ((filecount++))
done
