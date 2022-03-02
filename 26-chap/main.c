#include <inttypes.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

// Implementation where child processes are polled at 1Hz to
// identify completion.
void children_polling(void)
{
	printf("children_polling() Start\n");
	
	uint8_t i;
	for(i = 0; i < 3; i++)
	{
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
				// Child process, sleep then exit.
				sleep(5);
				_exit(0);
			}
			default:
			{
				printf("Child created - pid: %d\n", pid);
			}
		}
	}
	
	// Parent process, wait for all chilren to complete.
	printf("Wait for children");
	while(i > 0)
	{
		sleep(1);
		
		while(1)
		{
			pid_t pid = waitpid(-1, NULL, WNOHANG);
			if(pid == -1)
			{
				printf("\nError: waitpid()\n");
				return;
			}
			else if(pid == 0)
			{
				printf(".");
				break;
			}
			else
			{
				printf("\nChild complete - pid: %d", pid);
				i--;
				if(i == 0)
				{
					break;
				}
			}
		}
	}
	
	printf("\nchildren_polling()  Finished\n");
	return;
}

// Implementation where parent blocks until child processes complete
// execution.
void children_blocking(void)
{
	printf("children_blocking() Start\n");
	
	uint8_t i;
	for(i = 0; i < 3; i++)
	{
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
				// Child process, sleep then exit.
				sleep(5);
				_exit(0);
			}
			default:
			{
				printf("Child created - pid: %d\n", pid);
			}
		}
	}
	
	// Parent process, wait for all chilren to complete.
	printf("Wait for children");
	while(i > 0)
	{
		pid_t pid = waitpid(-1, NULL, 0);
		printf("\nChild complete - pid: %d", pid);
		i--;
	}
	
	printf("\nchildren_blocking() Finished\n");
	return;
}

volatile uint8_t i = 0;
void sigchld_handler(int sig)
{
	pid_t pid;
	while((pid = waitpid(-1, NULL, WNOHANG)) > 0)
	{
		printf("\nChild complete - pid: %d", pid);
		i--;
	}
}

// Implementation where child processes issue a signal to the parent
// to identify completion.  The parent executes a pause() in this
// example, but could be doing meaningful operation in the time
// it takes the children to complete.
void children_signalling(void)
{
	printf("children_signalling() Start\n");
	
	// Register the signal handler.
	struct sigaction sa;
	sa.sa_flags = 0;
	sa.sa_handler = &sigchld_handler;
	if(sigaction(SIGCHLD, &sa, NULL) == -1)
	{
		printf("Error: sigaction()\n");
	}
	
	for(i = 0; i < 3; i++)
	{
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
				// Child process, sleep then exit.
				sleep(3);
				_exit(0);
			}
			default:
			{
				printf("Child created - pid: %d\n", pid);
			}
		}
	}
	
	// Parent process, wait for all chilren to complete.
	printf("Wait for children");
	while(i > 0)
	{
		pause();
	}
	
	printf("\nchildren_signalling() Finished\n");
	return;
}

int main(int argc, char* argv[])
{
	//children_polling();
	//children_blocking();
	children_signalling();
	return(0);
}
