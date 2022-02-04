
#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

static void PrintClocks(void);
static void TimerCallback(union sigval sv);

int main(int argc, char* argv[])
{
	PrintClocks();
	
	printf("\n");
	printf("Sleep for 2.5 seconds...\n");
	printf("\n");
	struct timespec sleeptime;
	sleeptime.tv_sec = 2;
	sleeptime.tv_nsec = 500 * 1000 * 1000;
	clock_nanosleep(CLOCK_MONOTONIC, 0, &sleeptime, NULL);
	
	PrintClocks();
	
	// Setup timer to execute callback function as a new thread.
	timer_t timerid;
	struct sigevent sev;
	sev.sigev_notify = SIGEV_THREAD;
	sev.sigev_notify_function = &TimerCallback;
	sev.sigev_notify_attributes = NULL;
	if (timer_create(CLOCK_MONOTONIC, &sev, &timerid) != 0)
	{
		printf("ERROR: timer_create()\n");
	}
	
	// Set the timer to execute every 4 seconds.
	printf("\n");
	printf("Print clocks every 4.0 seconds for 12.0 seconds...\n");
	printf("\n");
	struct itimerspec tspec;
	tspec.it_interval.tv_sec = 4;
	tspec.it_interval.tv_nsec = 0;
	tspec.it_value.tv_sec = 4;
	tspec.it_value.tv_nsec = 0;
	if (timer_settime(timerid, 0, &tspec, NULL) != 0)
	{
		printf("ERROR: timer_gettime()\n");
	}
	
	// Let 4 second interval occur 3 times before exiting (3 * 4 + 1).
	sleeptime.tv_sec = 13;
	sleeptime.tv_nsec = 0;
	clock_nanosleep(CLOCK_MONOTONIC, 0, &sleeptime, NULL);
	if (timer_delete(timerid) != 0)
	{
		printf("ERROR: timer_gettime()\n");
	}
	
	// Verify timer doesn't occur again after being deleted.
	printf("\n");
	printf("Sleep for 5.0 seconds...\n");
	printf("\n");
	sleeptime.tv_sec = 5;
	sleeptime.tv_nsec = 0;
	clock_nanosleep(CLOCK_MONOTONIC, 0, &sleeptime, NULL);
	
	printf("\n");
	printf("All Done!\n");
	printf("\n");
	
	return 0;
}

static void PrintClocks(void)
{
	struct timespec tp_realtime,        res_realtime;
	struct timespec tp_monotonic,       res_monotonic;
	struct timespec tp_process_cputime, res_process_cputime;
	struct timespec tp_thread_cputime,  res_thread_cputime;
	
	// System-wide realtime clock.
	if (clock_gettime(CLOCK_REALTIME, &tp_realtime) != 0)
	{
		printf("ERROR: clock_gettime(CLOCK_REALTIME)\n");
	}
	if (clock_getres(CLOCK_REALTIME, &res_realtime) != 0)
	{
		printf("ERROR: clock_getres(CLOCK_REALTIME)\n");
	}
	
	// Clock that cannot be set and represents monotonic time since 
	// some unspecified starting point.
	if (clock_gettime(CLOCK_MONOTONIC, &tp_monotonic) != 0)
	{
		printf("ERROR: clock_gettime(CLOCK_MONOTONIC)\n");
	}
	if (clock_getres(CLOCK_MONOTONIC, &res_monotonic) != 0)
	{
		printf("ERROR: clock_getres(CLOCK_MONOTONIC)\n");
	}
	
	// High-resolution per-process timer from the CPU.
	if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tp_process_cputime) != 0)
	{
		printf("ERROR: clock_gettime(CLOCK_PROCESS_CPUTIME_ID)\n");
	}
	if (clock_getres(CLOCK_PROCESS_CPUTIME_ID, &res_process_cputime) != 0)
	{
		printf("ERROR: clock_getres(CLOCK_PROCESS_CPUTIME_ID)\n");
	}
	
	// Thread-specific CPU-time clock.
	if (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tp_thread_cputime) != 0)
	{
		printf("ERROR: clock_gettime(CLOCK_THREAD_CPUTIME_ID)\n");
	}
	if (clock_getres(CLOCK_THREAD_CPUTIME_ID, &res_thread_cputime) != 0)
	{
		printf("ERROR: clock_getres(CLOCK_THREAD_CPUTIME_ID)\n");
	}
	
	// Print out the different clock times.
	printf("CLOCK_REALTIME:\n");
	printf(" - time: %lld.%09ld\n", (int64_t)tp_realtime.tv_sec,  tp_realtime.tv_nsec);
	printf(" - res:  %lld.%09ld\n", (int64_t)res_realtime.tv_sec, res_realtime.tv_nsec);
	printf("CLOCK_MONOTONIC:\n");
	printf(" - time: %lld.%09ld\n", (int64_t)tp_monotonic.tv_sec,  tp_monotonic.tv_nsec);
	printf(" - res:  %lld.%09ld\n", (int64_t)res_monotonic.tv_sec, res_monotonic.tv_nsec);
	printf("CLOCK_PROCESS_CPUTIME_ID:\n");
	printf(" - time: %lld.%09ld\n", (int64_t)tp_process_cputime.tv_sec,  tp_process_cputime.tv_nsec);
	printf(" - res:  %lld.%09ld\n", (int64_t)res_process_cputime.tv_sec, res_process_cputime.tv_nsec);	
	printf("CLOCK_THREAD_CPUTIME_ID:\n");
	printf(" - time: %lld.%09ld\n", (int64_t)tp_thread_cputime.tv_sec,  tp_thread_cputime.tv_nsec);
	printf(" - res:  %lld.%09ld\n", (int64_t)res_thread_cputime.tv_sec, res_thread_cputime.tv_nsec);
}

static void TimerCallback(union sigval sv)
{
	PrintClocks();
	printf("\n");
	return;
}
