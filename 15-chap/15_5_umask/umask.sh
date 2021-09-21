#!/bin/bash

umask 0022
echo "Current umask: $(umask)"
echo
echo "Make a new file (using 'touch')"
touch tmp.txt
ls -al tmp.txt
echo
echo "Update umask..."
umask 0006
echo "New umask: $(umask)"
echo
echo "Make a new file (using 'touch')"
touch tmp2.txt
ls -al tmp2.txt
echo
rm tmp.txt
rm tmp2.txt