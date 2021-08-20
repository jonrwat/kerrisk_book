#!/bin/bash

echo
echo "~~~ All partition on the system ~~~"
echo
cat /proc/partitions

echo
echo "~~~ SD card partition details ~~~"
echo
sudo fdisk -l /dev/mmcblk0
