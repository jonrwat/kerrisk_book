#!/bin/bash

rm -rf dir
rm -rf hrd
rm -rf sym

reset

# Make some directories and files
mkdir dir
touch dir/file1
touch dir/file2

mkdir hrd
cd hrd
ln ../dir/file1 file1_ln
ln ../dir/file2 file2_ln
ln ../dir/file2 file2_ln2
cd ..

mkdir sym
cd sym
ln -s ../dir/file1 file1_sym
ln -s ../dir/file2 file2_sym
ln -s ../dir/file2 file2_sym2
cd ..

echo "~~~~~"
echo "~ DIRECTORY HEIRARCHY"
echo
ls -alR
echo
echo "~~~~~"

echo "file1 has two hardlinks"
stat dir/file1

echo "~~~~~"

echo "file2 has three hardlinks"
stat dir/file2

echo "~~~~~"

rm -v hrd/file1_ln
echo
echo "file1 has one hardlink after deleting one of them"
stat dir/file1

echo "~~~~~"
rm -v dir/file1

echo "sym/file1_sym is a dangling link after deleting all file1 hardlinks"
cat sym/file1_sym
echo
echo "list all dangling links:"
find . -xtype l
