#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	// Execute the interpreter script.
	execl("script.sh", "script.sh", (char*)NULL);
	
	printf("Error: execl() failure!\n");
	return EXIT_FAILURE;
}
