#define _DEFAULT_SOURCE

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>	// major(), minor() macro definitions.
#include <time.h>

#include <sys/types.h>

#define FILENAME "/dev/tty26"
//#define FILENAME "/home/pi/Source/15-chap/15_1_stat/main.c"

char* file_permission_string(uint16_t mode)
{
	static char mode_str[] = "---------";
  
	if (mode & S_IRUSR) mode_str[0] = 'r';
	if (mode & S_IWUSR) mode_str[1] = 'w';
	if (mode & S_IXUSR) mode_str[2] = 'x';
	if (mode & S_IRGRP) mode_str[3] = 'r';
	if (mode & S_IWGRP) mode_str[4] = 'w';
	if (mode & S_IXGRP) mode_str[5] = 'x';
	if (mode & S_IROTH) mode_str[6] = 'r';
	if (mode & S_IWOTH) mode_str[7] = 'w';
	if (mode & S_IXOTH) mode_str[8] = 'x';
	
	return mode_str;
}

int main(int argc, char* argv[])
{
	struct stat st;
	if (lstat(FILENAME, &st) != 0)
	{
		printf("ERROR: stat() - %s\n", strerror(errno));
		return EXIT_FAILURE;
	}
	
	printf("\n");
	printf("%s -- File Information:\n", FILENAME);
	printf("-------------------------------\n");

	printf("- Disk Device IDs (Major, Minor): %u, %u\n", major(st.st_dev), minor(st.st_dev));
	
	printf("- I-node number: %lu\n", st.st_ino);
	
	printf("- File permissions: %s", file_permission_string(st.st_mode));
	if (S_ISDIR(st.st_mode))  printf(" (directory)");
	if (S_ISCHR(st.st_mode))  printf(" (character special file)");
	if (S_ISBLK(st.st_mode))  printf(" (block special file)");
	if (S_ISREG(st.st_mode))  printf(" (regular file)");
	if (S_ISFIFO(st.st_mode)) printf(" (FIFO special file)");
	if (S_ISLNK(st.st_mode))  printf(" (symbolic link)");
	if (S_ISSOCK(st.st_mode)) printf(" (socket)");
	if (st.st_mode & S_ISUID) printf(" (set-user-id on execute)");
	if (st.st_mode & S_ISGID) printf(" (set-group-id on execute)");
	if (st.st_mode & S_ISVTX) printf(" (stick bit is set)");
	printf("\n");
	
	printf("- Number of hard-links to file: %u\n", st.st_nlink);
	
	printf("- File owner User ID:  %u\n", st.st_uid);
	printf("- File owner Group ID: %u\n", st.st_gid);
	
	printf("- Device Special File IDs (Major, Minor): %u, %u\n", major(st.st_rdev), minor(st.st_rdev));
	
	printf("- Total file size: %lu\n", st.st_size);
	
	printf("- Optimal block size for I/O: %lu\n", st.st_blksize);
	printf("- Number of 512b blocks allocated: %lu\n", st.st_blocks);
	
	printf("- ATIME: %s", ctime(&st.st_atime));	// Time of last file access.
	printf("- MTIME: %s", ctime(&st.st_mtime));	// Time of last file modification.
	printf("- CTIME: %s", ctime(&st.st_ctime));	// Time of last status change.
	
	printf("\n");
	printf("- ATIME (sec since epoch): %lu.%lu\n", st.st_atime, st.st_atim.tv_nsec);	// Time of last file access.
	printf("- MTIME (sec since epoch): %lu.%lu\n", st.st_mtime, st.st_mtim.tv_nsec);	// Time of last file modification.
	printf("- CTIME (sec since epoch): %lu.%lu\n", st.st_ctime, st.st_ctim.tv_nsec);	// Time of last status change.
	
	return EXIT_SUCCESS;
}
