#!/bin/bash

TTY26_ID_MAJOR=$(ls -l /dev/tty26 | awk -F'[, ]' '{ print $5 }')
TTY26_ID_MINOR=$(ls -l /dev/tty26 | awk -F'[, ]' '{ print $7 }')

echo "Device special files (aka device nodes) provide the user-space" 
echo "API to device drivers.  Each device special file has a"
echo "major,minor ID pair.  The major ID provides a mapping to"
echo "a specific device driver - i.e. the device special file's"
echo "filename has no correlation to the device driver used."
echo
echo "Device major ID mapping is managed by Lanana, documented here:"
echo "http://lxr.linux.no/linux+v2.6.31/Documentation/devices.txt"
echo
echo "Example device special file: /dev/tty26"
echo
echo "ls -l:"
ls -l /dev/tty26
echo
echo "Device Major ID: ${TTY26_ID_MAJOR}"
echo "Device Minor ID: ${TTY26_ID_MINOR}"
echo
echo "This cooresponds to file: /dev/char/${TTY26_ID_MAJOR}:${TTY26_ID_MINOR}"
echo
echo "ls -l /dev/char/${TTY26_ID_MAJOR}:${TTY26_ID_MINOR}"
ls -l /dev/char/${TTY26_ID_MAJOR}:${TTY26_ID_MINOR}
