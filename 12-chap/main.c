#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Problem 12.1 - List the process ID and command name for all
// processes being run by the user name listed in the command-line
// argument.
static int problem_12_1(int argc, char* argv[])
{
    // Verify program input.
    if (argc != 2)
    {
        printf("~~Error~~\n");
        printf("Usage: %s username\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Get passwd file information of selected user name.
    struct passwd* pwd_p;
    pwd_p = getpwnam(argv[1]);
    if (pwd_p == NULL)
    {
        printf("~~Error~~\n");
        printf("username not valid: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Open the /proc directory.
    DIR* dir;
    if ((dir = opendir("/proc/")) == NULL)
    {
        printf("~~Error~~\n");
        printf("Directory '/proc/' cannot be opened\n");
        return EXIT_FAILURE;
    }

    // Iterate through the /proc directory.
    struct dirent* dir_entry;
    while ((dir_entry = readdir(dir)) != NULL)
    {
        // First character in entry is a digit ?
        // - assume it's a process directory.
        if (isdigit(dir_entry->d_name[0]))
        {
            pid_t proc_pid       = 0;
            char  proc_name[256] = "\0";
            uid_t proc_uid       = 10;

            // Populate the process PID.
            proc_pid = strtoul(dir_entry->d_name, NULL, 10);

            // Build the full 'status' file name.
            char fn[512];
            sprintf(fn, "/proc/%d/status", proc_pid);

            // Open the 'status' file.
            FILE* fp = fopen(fn, "r");
            if (fp == NULL)
            {
                printf("~~Error~~\n");
                printf("fopen() failure\n");
                return EXIT_FAILURE;
            }

            // Parse the 'status' file.
            char ln[4096];
            char* token;
            while (fgets(ln, sizeof(ln), fp) != NULL)
            {
                token = strtok(ln, "\t");

                // Populate the process name if found.
                if (strcmp(token, "Name:") == 0)
                {
                    token = strtok(NULL, "\t");
                    strcpy(proc_name, token);
                }

                // Populate the process UID if found.
                if (strcmp(token, "Uid:") == 0)
                {
                    token = strtok(NULL, "\t");
                    proc_uid = strtoul(token, NULL, 10);
                }
            }

            // Close the 'status' file.
            fclose(fp);

            // Process being run by selected user ?
            if (proc_uid == pwd_p->pw_uid)
            {
                // Print the process information.
                printf("(%05d) %s", proc_pid, proc_name);
            }
        }
    }
    closedir(dir);

    return EXIT_SUCCESS;
}

int main(int argc, char* argv[])
{
    return problem_12_1(argc, argv);
    //~ return problem_12_2();
    //~ return problem_12_3();
}
