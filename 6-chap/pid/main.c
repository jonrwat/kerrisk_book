#define _GNU_SOURCE

#include <errno.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern char etext, edata, end;

extern char* program_invocation_name;
extern char* program_invocation_short_name;

extern char** environ;

jmp_buf jmp_env;

void jmp_f(void)
{
    printf("function jmp_f jumping\n");
    longjmp(jmp_env, 1);
}

int main(int argc, char* argv[])
{
    //-----------------------------
    // Hello World
    //-----------------------------
    {
        // Print 1st program greeting.
        printf("Hello World\n");
    }

    //-----------------------------
    // Process IDs
    //-----------------------------
    {
        // Print process ID.
        pid_t pid = getpid();
        printf("Process ID = %u\n", pid);

        // Print parent process ID.
        pid_t ppid = getppid();
        printf("Parent Process ID = %u\n", ppid);

        // Set a large timeout for viewing the process IDs
        // in a terminal.
        sleep(2);
    }

    //-----------------------------
    // Process Memory
    //-----------------------------
    {
        // Print processes virtual memory layout.
        //  - etext: Byte address after end of text segment (i.e. program text).
        //  - edata: Byte address after end of data segment (i.e. initialized global and static variables).
        //  - end:   Byte address after end of bss segment (i.e. uninitialized global and static variables).
        printf("etext: %p, edata: %p, end: %p\n", &etext, &edata, &end);

        // Print OS vitural memory page size.
        long psize = sysconf(_SC_PAGESIZE);
        printf("Virtual memory page size (bytes): %ld\n", psize);
    }

    //-----------------------------
    // Process Input (Command Line)
    //-----------------------------
    {
        // Print program invocation names.
        //  - The external strings are defined in errno after definition
        //    of `_GNU_SOURCE`.
        printf("\n");
        printf("Program invocation name: %s\n", program_invocation_name);
        printf("Program invocation short name: %s\n", program_invocation_short_name);

        // Print maximum size (in bytes) of command line arguments.
        long arg_max = sysconf(_SC_ARG_MAX);
        printf("\n");
        printf("Command line argument max-size (bytes): %ld\n", arg_max);
    }

    //-----------------------------
    // Process Input (Environment Variables)
    //-----------------------------
    {
        // Print a couple environment variables.
        printf("\n");
        printf("Environment Variables (from environ):\n");
        char** env;
        for (env = environ; *env != NULL; env++)
        {
            char* key;
            key = "USER=";
            if (memcmp(*env, key, strlen(key)) == 0)
            {
                printf("- %s\n", *env);
            }

            key = "HOME=";
            if (memcmp(*env, key, strlen(key)) == 0)
            {
                printf("- %s\n", *env);
            }

            // This is a custom environment variable.  It can be defined
            // by executing the application from `bash` in one of two ways:
            //  - JRW=Example ./main
            //  - env JRW=Example ./main
            key = "JRW=";
            if (memcmp(*env, key, strlen(key)) == 0)
            {
                printf("- %s\n", *env);
            }
        }

        char* key_value;
        printf("Environment Variables (from getenv):\n");
        if ((key_value = getenv("USER")) != NULL)
        {
            printf("- USER=%s\n", key_value);
        }

        if ((key_value = getenv("HOME")) != NULL)
        {
            printf("- HOME=%s\n", key_value);
        }

        // This is a custom environment variable.  It can be defined
        // by executing the application from `bash` in one of two ways:
        //  - JRW=Example ./main
        //  - env JRW=Example ./main
        if ((key_value = getenv("JRW")) != NULL)
        {
            printf("- JRW=%s\n", key_value);
        }

        // Set/modify some environment variables.
        setenv("JRW", "Modified", 1);
        setenv("JRW2", "WhatWhat", 0);

        printf("Set/Modified Environment Variables:\n");
        if ((key_value = getenv("JRW")) != NULL)
        {
            printf("- JRW=%s\n", key_value);
        }
        if ((key_value = getenv("JRW2")) != NULL)
        {
            printf("- JRW2=%s\n", key_value);
        }

        // Remove an environment variable.
        unsetenv("JRW");
        printf("Deleted Environment Variable JRW:\n");
        if ((key_value = getenv("JRW")) != NULL)
        {
            printf("- JRW=%s\n", key_value);
        }
        if ((key_value = getenv("JRW2")) != NULL)
        {
            printf("- JRW2=%s\n", key_value);
        }

        // Delete all an environment variable.
        clearenv();
        printf("Deleted ALL Environment Variable:\n");
        if ((key_value = getenv("JRW")) != NULL)
        {
            printf("- JRW=%s\n", key_value);
        }
        if ((key_value = getenv("JRW2")) != NULL)
        {
            printf("- JRW2=%s\n", key_value);
        }
    }

    //-----------------------------
    // Jump (Non-local GoTo)
    //-----------------------------
    {
        if (setjmp(jmp_env) == 0)
        {
            printf("\n");
            printf("setjmp setup\n");
            jmp_f();
        }
        else
        {
            printf("longjmp completed\n");
        }
    }

    return 0;
}
