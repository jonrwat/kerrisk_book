#!/bin/bash
#
# Mount option "user_xattr" is not present on the filesystem as identified by the book.  Most filesystems have this option enabled
# by default now though, so it isn't required to be selected:
#    https://unix.stackexchange.com/questions/290151/what-does-mounting-a-filesystem-with-user-xattr-do

FILE=tmp.txt

rm ${FILE}

echo
echo "Make tmp.txt.."
touch ${FILE}
echo "Add a couple example EAs..."
setfattr -n user.ex1 -v "We ride for Rohan" ${FILE}
setfattr -n user.ex2 -v "Not the beard" ${FILE}
echo
echo "Print ${FILE} EAs:"
getfattr -d ${FILE}
echo "Delete an attribute..."
setfattr -x user.ex1 ${FILE}
echo
echo "Print ${FILE} EAs:"
getfattr -d ${FILE}