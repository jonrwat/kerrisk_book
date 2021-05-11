
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    //------------------------------------------------------------------
    // Get the UNIX Epoch time.
    time_t time_tv;
    if (time(&time_tv) == -1)
    {
        printf("ERROR: time failure!\n");
        return EXIT_FAILURE;
    }
    
    struct timeval gettimeofday_tv;
    if (gettimeofday(&gettimeofday_tv, NULL) == -1)
    {
        printf("ERROR: gettimeofday failure!\n");
        return EXIT_FAILURE;
    }
    
    printf("Epoch time:\n");
    printf("- time(): %ld\n", time_tv);
    printf("- gettimeofday(): %ld.%ld\n", gettimeofday_tv.tv_sec, gettimeofday_tv.tv_usec);
    
    //------------------------------------------------------------------
    // Convert to broken-down time.
    struct tm gm_time;
    if (gmtime_r(&time_tv, &gm_time) == NULL)
    {
        printf("ERROR: gmtime failure!\n");
        return EXIT_FAILURE;
    }
    
    struct tm local_time;
    if (localtime_r(&time_tv, &local_time) == NULL)
    {
        printf("ERROR: localtime failure!\n");
        return EXIT_FAILURE;
    }
    
    printf("Broken-down time:\n");
    printf("- UTC (YYYY-MM-DD_HH-MM-SS): %d-%d-%d_%d-%d-%d\n",
        1900 + gm_time.tm_year, gm_time.tm_mon, gm_time.tm_mday,
        gm_time.tm_hour, gm_time.tm_min, gm_time.tm_sec);
    printf("- CST (YYYY-MM-DD_HH-MM-SS): %d-%d-%d_%d-%d-%d\n",
        1900 + local_time.tm_year, local_time.tm_mon, local_time.tm_mday,
        local_time.tm_hour, local_time.tm_min, local_time.tm_sec);
    
    //------------------------------------------------------------------
    // Convert to fixed-format time.
    char ctime_str[256];
    if (ctime_r(&time_tv, ctime_str) == NULL)
    {
        printf("ERROR: ctime failure!\n");
        return EXIT_FAILURE; 
    }
    
    char asctime_utc_str[256];
    if (asctime_r(&gm_time, asctime_utc_str) == NULL)
    {
        printf("ERROR: asctime UTC failure!\n");
        return EXIT_FAILURE; 
    }
    
    char asctime_cst_str[256];
    if (asctime_r(&local_time, asctime_cst_str) == NULL)
    {
        printf("ERROR: asctime CST failure!\n");
        return EXIT_FAILURE; 
    }
    
    printf("Fixed-format time:\n");
    printf("asctime(), UTC: %s", asctime_utc_str);
    printf("asctime(), CST: %s", asctime_cst_str);
    printf("ctime(),   CST: %s", ctime_str);
    
    //------------------------------------------------------------------
    // Convert to user-formatted time.
    char strftime_utc_str[256];
    if (strftime(strftime_utc_str, sizeof(strftime_utc_str), "%Y-%m-%d_%H-%M-%S", &gm_time) == 0)
    {
        printf("ERROR: strftime UTC failure!\n");
        return EXIT_FAILURE; 
    }
    
    char strftime_cst_str[256];
    if (strftime(strftime_cst_str, sizeof(strftime_cst_str), "%Y-%m-%d_%H-%M-%S", &local_time) == 0)
    {
        printf("ERROR: strftime UTC failure!\n");
        return EXIT_FAILURE; 
    }
    
    printf("User-format time:\n");
    printf("strftime(), UTC: %s\n", strftime_utc_str);
    printf("strftime(), CST: %s\n", strftime_cst_str);
    
    //------------------------------------------------------------------
    // Get the current timezone.
    
    // Initialize the timezone global variables - note: should already
    // be performed before internal to time-based functions, but done 
    // here to document feature.
    tzset();
    
    // Print tzname global variable (from time.h).
    printf("Current Timezone:\n");
    printf("tzname(): %s\n", tzname[1]);
    
    //------------------------------------------------------------------
    // Get the current locale.
    char* locale_str = setlocale(LC_ALL, NULL);

    printf("Current Locale:\n");
    printf("setlocale(): %s\n", locale_str);
    
    //------------------------------------------------------------------
    // Get the current time adjustment (e.g. NTP adjustment).
    struct timeval adj;
    if (adjtime(NULL, &adj) == -1)
    {
        printf("ERROR: adjtime read failure!\n");
        return EXIT_FAILURE; 
    }
    
    printf("Current Time Adjustment:\n");
    printf("- adjtime(): %ld.%ld\n", adj.tv_sec, adj.tv_usec);
    

    //------------------------------------------------------------------
    // Set small time adjustment - 1/2 second.
    adj.tv_sec = 0;
    adj.tv_usec = 500000;
    if (adjtime(&adj, NULL) == -1)      // NOTE: RUN PROGRAM AS "sudo ./main" TO HAVE CORRECT PRIVLEDGES.
    {
        printf("ERROR: adjtime write failure!\n");
        return EXIT_FAILURE; 
    }
    
    printf("Set Time Adjustment:\n");
    printf("- adjtime(): %ld.%ld\n", adj.tv_sec, adj.tv_usec);
    
    //------------------------------------------------------------------
    // Read the small time adjustment - after waiting a couple seconds
    // to see that the system is actively making small adjustments to 
    // the system time.
    usleep(4000000);
    if (adjtime(NULL, &adj) == -1)
    {
        printf("ERROR: adjtime read failure!\n");
        return EXIT_FAILURE; 
    }
    
    printf("Read Time Adjustment:\n");
    printf("- adjtime(): %ld.%ld\n", adj.tv_sec, adj.tv_usec);
    
    //------------------------------------------------------------------
    // Perform a jump of system time 1 minute into the future.
    
    printf("Jump the System Time:\n");
    struct timeval tv;
    if (gettimeofday(&tv, NULL) == -1)
    {
        printf("ERROR: gettimeofday() failure!\n");
        return EXIT_FAILURE;
    }
    printf("- gettimeofday(): %ld.%ld\n", tv.tv_sec, tv.tv_usec);
    
    printf("- Jump 1 minute ...\n");
    tv.tv_sec += 60;
    if (settimeofday(&tv, NULL) == -1)
    {
        printf("ERROR: settimeofday failure!\n");
        return EXIT_FAILURE;
    }
    
    if (gettimeofday(&tv, NULL) == -1)
    {
        printf("ERROR: gettimeofday() failure!\n");
        return EXIT_FAILURE;
    }
    printf("- gettimeofday(): %ld.%ld\n", tv.tv_sec, tv.tv_usec);


    //------------------------------------------------------------------
    // Display used CPU time (user and kernel) of process.
    
    clock_t clock_ticks = clock();
    if (clock_ticks == -1)
    {
        printf("ERROR: clock() failure!\n");
        return EXIT_FAILURE;
    }
    printf("Process CPU Utilization Time:\n");
    printf("- clock(): %f seconds\n", (double)clock_ticks / CLOCKS_PER_SEC);
}
