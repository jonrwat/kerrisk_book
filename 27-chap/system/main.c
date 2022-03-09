#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	printf("Application Start!\n");
	printf("\n");
	
	int ret = system("ls -l");
	printf("\n");
	if (ret == -1)
	{
		printf("Error: Child process failure!\n");
	}
	else if (WIFEXITED(ret) && (WEXITSTATUS(ret) == 127))
	{
		printf("Error: Shell unable to be executed!\n");
	}
	else
	{
		printf("Success: 'ls -l' termination value: %d\n", ret);
	}
	
	printf("Application Finished!\n");
	return EXIT_SUCCESS;
}
