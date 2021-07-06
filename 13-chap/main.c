#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// This function write a file (in chunks) using the unistd.h "write()" function.
//
// Execution of function with "time" Linux command produces the following:
//	Asyncronous write (i.e. kernel buffered I/O):
//		real	0m0.090s
//		user	0m0.001s
//		sys		0m0.089s
//	Synchornous write (i.e. O_SYNC, synchronized I/O):
//		real	0m13.580s
//		user	0m0.001s
//		sys		0m0.705s
//  Asyncronous write w/ fsync after each write:
//		real	0m15.563s
//		user	0m0.011s
//		sys		0m0.710s
//
// Timing performance shows the motivation for kernel buffering of I/O 
// disk access - i.e. significant time is required for disk access. As
// expected a synchonous write (O_SYNC) is equivalent to an asynchronous
// write with a following fsync.
void write_file(void)
{
	char*  STR     = "This works I think!\n";
	size_t STR_LEN = strlen(STR);
	
	// Open file for writing.
	// - O_SYNC: synchronized I/O file integrity completion - i.e. file save to disk before "write()" returns.
	int fd = open("/home/pi/Source/13-chap/tmp.txt", O_WRONLY | O_CREAT);
	if (fd == -1)
	{
		printf("Error: open()\n");
		exit(EXIT_FAILURE);
	}
	
	// Write the file content.
	uint32_t cnt;
	for (cnt = 0; cnt < 10000; cnt++)
	{
		if (write(fd, STR, STR_LEN) != STR_LEN)
		{
			printf("Error: write()\n");
			exit(EXIT_FAILURE);
		}
		
		if (fsync(fd) != 0)
		{
			printf("Error: fsync()\n");
			exit(EXIT_FAILURE);
		}
	}
	
	// Close the file.
	if (close(fd) != 0)
	{
		printf("Error: close()\n");
		exit(EXIT_FAILURE);
	}
	
	return;
}

// This function write a file (in chunks) using the stdio.h "fprintf()" function.
// The C library I/O functions (e.g. fprintf) performs buffering of data into
// larger chuncks to reduce kernel system calls (which improved timing performance).
//
// Execution of function with "time" Linux command produces the following:
//	_IOFBF (full buffering):
// 		real	0m0.021s
// 		user	0m0.000s
// 		sys		0m0.020s
//	_IOLBF (line buffering):
// 		real	0m0.124s
// 		user	0m0.028s
// 		sys		0m0.091s
//	_IONBF (no buffering):
// 		real	0m0.319s
// 		user	0m0.011s
// 		sys		0m0.306s
//	_IOFBF (full buffering) but with fflush after each fprintf:
// 		real	0m0.329s
// 		user	0m0.010s
// 		sys		0m0.316s
// Timing performance shows the motivation for user-space buffering to 
// reduce the number of system calls (kernel accesses).  As expected,
// no buffering (_IONBF) and buffering but with a fflush after each
// fprintf are equivalent in performance.
void print_file(void)
{
	char*  STR1 = "This ";
	char*  STR2 = "works ";
	char*  STR3 = "I ";
	char*  STR4 = "think!\n";
	
	// Open file for writing.
	FILE* fd = fopen("/home/pi/Source/13-chap/tmp.txt", "w");
	if (fd == NULL)
	{
		printf("Error: open()\n");
		exit(EXIT_FAILURE);
	}
	
	// Set the file buffering mode.
	//	- _IONBF: I/O no buffering.
	//	- _IOLBF: I/O line buffering (newline delimiting).
	//	- _IOFBF: I/O full buffering (buffered for full buffer size).
	char buf[BUFSIZ];
	if (setvbuf(fd, buf, _IOFBF, sizeof(buf)) != 0)
	{
		printf("Error: setvbuf()\n");
		exit(EXIT_FAILURE);
	}
	
	// Write the file content.
	uint32_t cnt;
	for (cnt = 0; cnt < 10000; cnt++)
	{
		fprintf(fd, "%s", STR1);
		// fflush(fd);
		fprintf(fd, "%s", STR2);
		// fflush(fd);
		fprintf(fd, "%s", STR3);
		// fflush(fd);
		fprintf(fd, "%s", STR4);
		// fflush(fd);
	}
	
	// Close the file.
	if (fclose(fd) != 0)
	{
		printf("Error: close()\n");
		exit(EXIT_FAILURE);
	}
	
	return;
}

// Figure 13-1 provides a summary of the chapter - buffering is performed
// at two levels:
//	- user-space (C stdio): to reduce the number of kernel system calls.
//	- kernel-space (for disk access): to reduce the number of hardware accesses.
int main(int argc, char* argv[])
{
	write_file();
	return EXIT_SUCCESS;
}
