#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>

int main(int argc, char* argv[])
{
	if (mount(NULL, "/", NULL, MS_REMOUNT|MS_RELATIME, NULL) != 0)
	{
		printf("mount() failure!  Errno: %s\n\n", strerror(errno));
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}

/*
NOTE: EXECUTE USING "sudo ./main
 
This application remounts the root filesystem to change the mount from
"noatime" to "relatime".  The RPi4 default is to have the filesystem
with "noatime" which results in the atime (i.e. access time) of files
never updating.  Remounting with "relatime" causes the atime to update
when the file is accessed, if the atime is less than the file modification
time (mtime) or file status change time (ctime).  The following sequence
of commands on the remounted filesystem show this operation.


pi@raspberrypi:~/Source/14-chap/rootfs_update $ echo "yes" > tmp.txt
pi@raspberrypi:~/Source/14-chap/rootfs_update $ stat tmp.txt 
  File: tmp.txt
  Size: 4         	Blocks: 8          IO Block: 4096   regular file
Device: b307h/45831d	Inode: 535293      Links: 1
Access: (0644/-rw-r--r--)  Uid: ( 1000/      pi)   Gid: ( 1000/      pi)
Access: 2021-08-04 14:52:07.190988809 -0500
Modify: 2021-08-04 14:52:07.190988809 -0500
Change: 2021-08-04 14:52:07.190988809 -0500
 Birth: -
pi@raspberrypi:~/Source/14-chap/rootfs_update $ echo "no" > tmp.txt
pi@raspberrypi:~/Source/14-chap/rootfs_update $ stat tmp.txt 
  File: tmp.txt
  Size: 3         	Blocks: 8          IO Block: 4096   regular file
Device: b307h/45831d	Inode: 535293      Links: 1
Access: (0644/-rw-r--r--)  Uid: ( 1000/      pi)   Gid: ( 1000/      pi)
Access: 2021-08-04 14:52:07.190988809 -0500
Modify: 2021-08-04 14:52:20.530938003 -0500
Change: 2021-08-04 14:52:20.530938003 -0500
 Birth: -
pi@raspberrypi:~/Source/14-chap/rootfs_update $ cat tmp.txt 
no
pi@raspberrypi:~/Source/14-chap/rootfs_update $ stat tmp.txt 
  File: tmp.txt
  Size: 3         	Blocks: 8          IO Block: 4096   regular file
Device: b307h/45831d	Inode: 535293      Links: 1
Access: (0644/-rw-r--r--)  Uid: ( 1000/      pi)   Gid: ( 1000/      pi)
Access: 2021-08-04 14:52:41.000860191 -0500
Modify: 2021-08-04 14:52:20.530938003 -0500
Change: 2021-08-04 14:52:20.530938003 -0500
 Birth: -

*/
