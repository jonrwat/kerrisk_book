#!/bin/bash

echo
echo "~~~ Filesystem types recognized by Kernetl ~~~"
echo
cat /proc/filesystems

echo "~~~ Root filesystem detail ~~~"
echo
sudo dumpe2fs -h /dev/mmcblk0p7