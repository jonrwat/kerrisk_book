#include <limits.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	// Configuration limits and options can be acceessed from multiple
	// locations.
	// - <limits.h> includes limits with a _POSIX_ prefix which definces
	//   the minimum SUSv3 defined limit.  That is, all POSIX compliant
	//   operating systems will have a maximum equal to or larger than
	//   the _POSIX_ limit.  Therefore, an application which uses the
	//	 _POSIX_ limit is guaranteed to execute correctly on all
	//   POSIX operating systems.  The POSIX limit is usually quite 
	//	 limiting though and doesn't allow an application to take full
	//	 advantage of the implementations limit.
	// - Limits without the _POSIX_ suffix are implementation limits -
	//	 i.e. the limit for the POSIX operating system which is executing
	//   the application.  Using this limit allow the application to fully
	//   utilize the capabilities of the operating system.
	// - Runtime invariant values (i.e. fixed values) can be accessed from
	//	 either limits.h or sysconf/pathconf().
	// - Variable/non-fixed values (which can only increase with time)
	//   must be accessed using sysconf().
	// - Variable pathname values (which are different based on the supplied
	//   pathname in the filesystem) must be accessed using pathconf()/fpathconf().
	
	printf("LOGIN_NAME Limit (invariant/fixed value):\n");
	printf("- POSIX:     %d\n", _POSIX_LOGIN_NAME_MAX);
	printf("- limits.h:  %d\n", LOGIN_NAME_MAX);
	printf("- sysconf(): %ld\n", sysconf(_SC_LOGIN_NAME_MAX));
	
	printf("ARG_MAX Limit (variable/runtime-increasing value):\n");
	printf("- POSIX:     %d\n", _POSIX_ARG_MAX);
	// printf("- limits.h:  %d\n", ARG_MAX);	ARG_MAX not defined in limits.h since it's a runtime-increasing value.
	printf("- sysconf(): %ld\n", sysconf(_SC_ARG_MAX));
	
	printf("PATH_MAX Limit (pathname variable value):\n");
	printf("- POSIX:      %d\n", _POSIX_PATH_MAX);
	printf("- pathconf(): %ld\n", pathconf("/", _PC_PATH_MAX));
	
	// System options specify the options that a UNIX implementation may support.
	// - A value of "200809" specifies the year and month of SUSv4
	// being approved as a standard. And identifies that the implementation
	// supports the option.

	printf("THREADS Option:\n");
	printf("- POSIX:     %ld\n", _POSIX_THREADS);
	printf("- sysconf(): %ld\n", sysconf(_SC_THREADS));
}
