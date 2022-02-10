#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	int tmp1_fd;
	int tmp2_fd;
	
	char* tmp1_str;
	char* tmp2_str;

	tmp1_fd = open("tmp1.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	if (tmp1_fd == -1)
	{	
		printf("open(tmp1.txt) failed!\n");
		return EXIT_FAILURE;
	}
	
	switch(fork())
	{
		case -1:
		{
			printf("fork() failed!\n");
			return EXIT_FAILURE;
		}
		case 0:
		{
			tmp2_fd = open("tmp2.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
			if (tmp2_fd == -1)
			{	
				printf("child: open(tmp2.txt) failed!\n");
				return EXIT_FAILURE;
			}
			tmp1_str = "---\n";
			tmp2_str = "---\n";
			break;
		}
		default:
		{
			tmp2_fd = open("tmp2.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
			if (tmp2_fd == -1)
			{	
				printf("parent: open(tmp2.txt) failed!\n");
				return EXIT_FAILURE;
			}
			tmp1_str = "*****\n";
			tmp2_str = "*****\n";
			break;
		}
	}
		
	// Print out data to tmp1.txt and tmp2.txt.
	//
	//	Inspection of the generated files shows that "tmp1.txt" is an
	//  intelligent output (i.e., not corrupted).  Both processes write
	//  to the file but since the file description between the two 
	//  processes is identical (i.e., they share a file offset in the
	//  kernel open file table - Figure 5-2) one process doesn't
	//  overwrite the output of the other process.
	//
	//  Inspection of the generated files shows that "tmp2.txt" is
	//  corrupted.  Since both processes opened the file indepently
	//  (i.e., they don't share a file offset) one process is 
	//  overwritting the output of the other process.  This could be
	//  resolved by synchronizing the processes so that they don't
	//  write the same file concurrently, or my using the open() flag
	//  O_APPEND.
	//
	int i;
	for (i = 0; i < 100; i++)
	{
		(void) write(tmp1_fd, tmp1_str, strlen(tmp1_str));
		(void) write(tmp2_fd, tmp2_str, strlen(tmp2_str));
	}
	
	return EXIT_SUCCESS;
}
