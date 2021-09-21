#!/bin/bash

FILE="tmp.txt"

rm ${FILE}

echo "Make temporary file (using 'touch')"
touch ${FILE}
echo
echo "File attributes:"
lsattr ${FILE}
echo
echo "Update file contents..."
echo "Example file contents" > ${FILE}
echo "File contents:"
cat ${FILE}
echo
echo "Make file immutable..."
sudo chattr +a ${FILE}
echo "File attributes:"
lsattr ${FILE}
echo
echo "Attempt to overwrite the file..."
echo "New content" > ${FILE}
echo
echo "Attempt to append to file..."
echo "New content" >> ${FILE}
echo "File contents:"
cat ${FILE}
echo
echo
echo "Note: you can't delete IMMUTABLE or APPEND files"
rm ${FILE}
echo
echo "You have to remove this attribute first"
echo "Removing append attribute..."
sudo chattr -a ${FILE}
rm -v ${FILE}
echo