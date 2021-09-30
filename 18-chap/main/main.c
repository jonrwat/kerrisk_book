//
// To execute application from terminal run:
//
//   sudo rm -rf dir/; sudo ./main
//


// For ftw.h
#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <ftw.h>
#include <libgen.h>
#include <linux/limits.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int nftw_fxn(const char *fpath, 
			 const struct stat *sb,
             int typeflag, 
             struct FTW *ftwbuf)
{
	// Regular file ?
	if ((typeflag == FTW_F) && S_ISREG(sb->st_mode))
	{
		// Append ", file updated" to end of file contents.
		FILE *fptr;
		if ((fptr = fopen(fpath,"a")) == NULL)
		{
			printf("fopen() failure!\n");
			return EXIT_FAILURE;
		}
		fprintf(fptr, ", file updated");
		fclose(fptr);
	}
	
	// return zero to continue the tree walk.
	return 0;
}

int main(int argc, char* argv[])
{
	// Make directories and files for testing. -------------------------
	if (mkdir("dir", 0755) != 0)
	{
		printf("mkdir() failure!\n");
		return EXIT_FAILURE;
	}
	
	if (mkdir("dir/sub_dir", 0755) != 0)
	{
		printf("mkdir() failure!\n");
		return EXIT_FAILURE;
	}
	
	FILE *fptr;
	if ((fptr = fopen("dir/file1.txt","w")) == NULL)
	{
		printf("fopen() failure!\n");
		return EXIT_FAILURE;
	}
	fprintf(fptr, "file1 text");
	fclose(fptr);
	
	if ((fptr = fopen("dir/sub_dir/file2.txt","w")) == NULL)
	{
		printf("fopen() failure!\n");
		return EXIT_FAILURE;
	}
	fprintf(fptr, "file2 text");
	fclose(fptr);
	
	if ((fptr = fopen("dir/sub_dir/file3.txt","w")) == NULL)
	{
		printf("fopen() failure!\n");
		return EXIT_FAILURE;
	}
	fprintf(fptr, "file3 text");
	fclose(fptr);
	
	// Perform File Tree Walk (nftw) to update all file contents. ------
	
	if (nftw("dir", &nftw_fxn, 20, 0) == -1)
	{
		printf("nftw() failure!\n");
		return EXIT_FAILURE;
	}
	
	// Print components of a pathname. ---------------------------------
	char* main_pathname = "/home/pi/Source/18-chap/main/main.c";

	printf("\n");
	printf("main.c Pathname: %s\n", main_pathname);
	
	// dirname() and basename() can modify the passed source string
	// so make duplicates.
	char* main_pathname_dup1 = strdup(main_pathname);
	if (main_pathname_dup1 == NULL)
	{
		printf("strdup() failure!\n");
		return EXIT_FAILURE;
	}
	char* main_pathname_dup2 = strdup(main_pathname);
	if (main_pathname_dup2 == NULL)
	{
		printf("strdup() failure!\n");
		return EXIT_FAILURE;
	}
	
	printf("main.c Directory: %s\n", dirname(main_pathname_dup1));
	printf("main.c Filename: %s\n", basename(main_pathname_dup2));
	
	free(main_pathname_dup1);
	free(main_pathname_dup2);
	
	// Print CWD and ROOT from process files. --------------------------
	printf("\n");
	int pid = getpid();
	
	char cwd_filename[PATH_MAX];
	char root_filename[PATH_MAX];
	
	sprintf(cwd_filename, "/proc/%d/cwd", pid);
	sprintf(root_filename, "/proc/%d/root", pid);
	
	char buf[256];
	if (readlink(cwd_filename, buf, sizeof(buf)) == -1)
	{
		printf("readlink() failure: %s\n", cwd_filename);
		sleep(30);
		return EXIT_FAILURE;
	}
	printf("%s: %s\n", cwd_filename, buf);
	
	if (readlink(root_filename, buf, sizeof(buf)) == -1)
	{
		printf("readlink() failure: %s\n", root_filename);
		return EXIT_FAILURE;
	}
	printf("%s: %s\n", root_filename, buf);
	
	// Change CMD and ROOT----------------------------------------------
	char cwd[PATH_MAX];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		printf("getcwd() failure!\n");
		return EXIT_FAILURE;
	}
	printf("\n");
	printf("CWD: %s\n", cwd);
	
	strcat(cwd, "/dir");
	if (chdir(cwd) == -1)
	{
		printf("chdir() failure!\n");
		return EXIT_FAILURE;
	}
	
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		printf("getcwd() failure!\n");
		return EXIT_FAILURE;
	}
	printf("New CWD: %s\n", cwd);
	
	// Open /etc/passwd.
	char* passwd_filename = "/etc/passwd";
	if ((fptr = fopen(passwd_filename,"r")) == NULL)
	{
		printf("fopen() failure!\n");
		return EXIT_FAILURE;
	}
	else
	{
		printf("\n");
		printf("fopen() sucessful: %s\n", passwd_filename);
	}
	fclose(fptr);
	
	// Change root.
	if (chroot("/etc") == -1)
	{
		printf("chroot() failure!\n");
		return EXIT_FAILURE;
	}
	else
	{
		printf("Root changed to: /etc\n");
	}
	
	// Try opening /etc/passwd again - use '/passwd' since '/etc' is 
	// now the root directory.
	if ((fptr = fopen("/passwd","r")) == NULL)
	{
		printf("fopen() failure: %s\n", "/passwd");
		printf("%s\n",strerror(errno));
		return EXIT_FAILURE;
	}
	else
	{
		printf("fopen() sucessful: %s\n", "/passwd");
		fclose(fptr);
	}

	return EXIT_SUCCESS;
}
