
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

// Perform an exec() from the calling process.  A new program is loaded
// and return from the exec() should never occur.
//~ int main(int argc, char* argv[])
//~ {
	//~ // Execute an "ls" command with long (-l) form.
	//~ //  - first program argument is always the command.
	//~ //  - exec() should not return.
	//~ execl("/bin/ls", "/bin/ls", "-l", (char*)NULL);
	//~ printf("Error: execl()\n");
	
	//~ return EXIT_FAILURE;
//~ }


// Perform an exec() from a child process.  This allows the parent
// to execute additional functionality, and monitor the child's
// completion.  Blocking on child completion performed in the case
// for simplicity, but polling or signalling could otherwise be used.
int main(int argc, char* argv[])
{
	printf("Application Start\n");

	pid_t pid = fork();
	switch(pid)
	{
		case -1:
		{
			printf("Error: fork()!\n");
			break;
		}
		case 0:
		{
			// Child process, exec() a new program.
			execl("/bin/ls", "/bin/ls", "-l", (char*)NULL);
			printf("Error: execl()\n");
			_exit(0);
		}
		default:
		{
			printf("Child created - pid: %d\n", pid);
		}
	}
	
	// Parent process, wait for child to complete.
	printf("Wait for child\n");
	printf("\n");
	pid = waitpid(-1, NULL, 0);
	printf("\n");
	printf("Child complete - pid: %d\n", pid);
	printf("Application Finished\n");
	
	return EXIT_SUCCESS;
}
