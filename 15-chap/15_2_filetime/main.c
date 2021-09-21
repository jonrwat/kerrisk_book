#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define PATHNAME "/home/pi/Source/15-chap/15_2_filetime/tmp.txt"

int main(int argc, char* argv[])
{
	// Set the file timestamps to the current time.
	struct timespec times[2];
	times[0].tv_sec  = 0;
	times[0].tv_nsec = UTIME_NOW;
	times[1].tv_sec  = 0;
	times[1].tv_nsec = UTIME_NOW;
	utimensat(AT_FDCWD, PATHNAME, times, 0);
	
	// Print the file's timestamps.
	struct stat st;
	if (stat(PATHNAME, &st) != 0)
	{
		printf("ERROR: stat()\n");
		return EXIT_FAILURE;
	}
	printf("Set file timestamps to NOW:\n");
	printf("ATIME: %lu.%lu\n", st.st_atime, st.st_atim.tv_nsec);
	printf("MTIME: %lu.%lu\n", st.st_mtime, st.st_mtim.tv_nsec);
	printf("CTIME: %lu.%lu\n", st.st_ctime, st.st_ctim.tv_nsec);
	
	// Update file to cool nano-seconds
	times[0].tv_sec  = st.st_atime;
	times[0].tv_nsec = 123456789;
	times[1].tv_sec  = st.st_mtime;
	times[1].tv_nsec = 987654321;
	utimensat(AT_FDCWD, PATHNAME, times, 0);
	
	// Print the file's timestamps again.
	if (stat(PATHNAME, &st) != 0)
	{
		printf("ERROR: stat()\n");
		return EXIT_FAILURE;
	}
	printf("\nUpdate nano in file timestamp to cool values:\n");
	printf("ATIME: %lu.%lu\n", st.st_atime, st.st_atim.tv_nsec);
	printf("MTIME: %lu.%lu\n", st.st_mtime, st.st_mtim.tv_nsec);
	printf("CTIME: %lu.%lu\n", st.st_ctime, st.st_ctim.tv_nsec);
	
	return EXIT_SUCCESS;
}

