#define _DEFAULT_SOURCE
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void SigintHandler(int sig)
{
    static uint8_t cnt = 1;
    
    if (cnt <= 3)
    {
        printf("Nope #%u: Signal %s\n", cnt, strsignal(sig));
        cnt++;
    }
    else
    {
        printf("OK I'll stop.\n");
        
        // Retrun SIGINT to default behavior (i.e., terminate/kill process).
        struct sigaction sa;
        sa.sa_handler = SIG_DFL;
        if (sigaction(SIGINT, &sa, NULL) != 0)
        {
            fprintf(stderr, "sigaction() failure!\n");
        }
    }
}

int main(int argc, char* argv[])
{
    printf("Ignore Control-C for 5 seconds...\n");
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    if (sigaction(SIGINT, &sa, NULL) != 0)
    {
        fprintf(stderr, "sigaction() failure!\n");
        return EXIT_FAILURE;
    }
    sleep(5);
    
    printf("\nControl-C available (3 attempts required)...\n");
    sa.sa_handler = SigintHandler;
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGINT, &sa, NULL) != 0)
    {
        fprintf(stderr, "sigaction() failure!\n");
        return EXIT_FAILURE;
    }
    
    while(1)
    {
        // Control-C required for exit.
    }
    
    return EXIT_SUCCESS;
}
