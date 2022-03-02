#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void exit_handler1(void)
{
	printf ("-- exit handler_1 ");
}

void exit_handler2(void)
{
	printf ("-- exit handler_2 ");
}

int main(int argc, char* argv[])
{
	if (atexit(&exit_handler1) != 0)
	{
		printf("ERROR: atexit(&exit_handler1)/n");
	}
	if (atexit(&exit_handler2) != 0)
	{
		printf("ERROR: atexit(&exit_handler2)/n");
	}
	
	printf("-- main ");
	
	switch(fork())
	{
		case -1:
		{
			printf("ERROR: fork()/n");
			break;
		}
		case 0:
		{
			printf("-- child ");
			//~ fflush(stdout);
			//~ _exit(0);
			break;
		}
		default:
		{
			printf("-- parent ");
			break;
		}
	}
	
	exit(0);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Assessment:
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Generated Output:
//		-- main -- parent -- exit handler_2 -- exit handler_1 -- main -- child -- exit handler_2 -- exit handler_1 
//
// The output identifies many interestion topics, including:
//	1. Exit handlers are executed in the reverse order in which they
//     are registered.  This is similar to the C++ object constructor
//     and destructor process, as later elements in the constuctor may
//     rely on previous constructor elements.
//
//	2. stdio buffers are copied when forking a child.  Performing an
//	   `exit()` within the child causes these duplicate buffers to be
//	   flushed.  An `_exit()` could instead be performed to not have
//	   the stdio buffers flushed.
//
//  3. Exit handlers are duplicated by a fork() and therefore are
//     executed by the child on `exit()` execution.
//  
