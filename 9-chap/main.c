#define _GNU_SOURCE

#include <grp.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// IMPORTANT -----------------------------------------------------------
// After comiling the program, set the executable to "root" user and
// group ownership, and set the set-user-ID and set-group-id bits.
// Accomplish this my executing script "update-priv.sh".
// ---------------------------------------------------------------------

// NOTE ----------------------------------------------------------------
// User-ID and group-ID for a process can be identified in file
// /proc/#/status.  The following man page identifies that the four
// fields for the "Uid" and "Gid" are: real, effective, saved, and
// filesystem identifiers.
//	@see https://man7.org/linux/man-pages/man5/proc.5.html
// ---------------------------------------------------------------------

int main(int argc, char* argv[])
{
	printf("Real user-ID:       %d\n", getuid());
	printf("Real group-ID:      %d\n", getgid());
	printf("Effective user-ID:  %d\n", geteuid());
	printf("Effective group-ID: %d\n", getegid());
	printf("\n");
	
	// -----------------------------------------------------------------
	// Set real user-ID and group-ID
	{
		uid_t new_uid = 0;
		if (setuid(new_uid) == -1)
		{
			printf("ERROR: Unable to update Real user-ID to: %d\n", new_uid);
			return EXIT_FAILURE;
		}
		
		uid_t new_gid = 0;
		if (setgid(new_gid) == -1)
		{
			printf("ERROR: Unable to update Real group-ID to: %d\n", new_gid);
			return EXIT_FAILURE;
		}
		
		printf("Real user-ID and group-ID updated ...\n");
		printf("Real user-ID:       %d\n", getuid());
		printf("Real group-ID:      %d\n", getgid());
		printf("Effective user-ID:  %d\n", geteuid());
		printf("Effective group-ID: %d\n", getegid());
		printf("\n");
	}
	
	// -----------------------------------------------------------------
	// Set effective user-ID and group-ID
	// Note: effective group-ID needs to be set to unprivledged (non-root)
	// value before setting the effective user-ID; otherwise the process 
	// won't have the required privledged to set the effective group-ID.
	{
		uid_t new_egid = 1000;
		if (setegid(new_egid) == -1)
		{
			printf("ERROR: Unable to update Effective group-ID to: %d\n", new_egid);
			return EXIT_FAILURE;
		}
		
		uid_t new_euid = 1000;
		if (seteuid(new_euid) == -1)
		{
			printf("ERROR: Unable to update Effective user-ID to: %d\n", new_euid);
			return EXIT_FAILURE;
		}
		
		printf("Effective user-ID and group-ID updated ...\n");
		printf("Real user-ID:       %d\n", getuid());
		printf("Real group-ID:      %d\n", getgid());
		printf("Effective user-ID:  %d\n", geteuid());
		printf("Effective group-ID: %d\n", getegid());
		printf("\n");
	}
	
	// -----------------------------------------------------------------
	// Print the saved user-ID and group-ID
	// Note: These functions are non-standard and non-portable.  This
	// is the reason for required macro "_GNU_SOURCE" at the top of
	// this file.
	{
		uid_t ruid, euid, suid;
		gid_t rgid, egid, sgid;
		int getresuid_ret = getresuid(&ruid, &euid, &suid);
		int getresgid_ret = getresgid(&rgid, &egid, &sgid);
		if (getresuid_ret == -1)
		{
			printf("ERROR: getresuid_ret failure\n");
			return EXIT_FAILURE;
		}
		if (getresgid_ret == -1)
		{
			printf("ERROR: getresgid failure\n");
			return EXIT_FAILURE;
		}
		
		printf("Real user-ID:       %d\n", getuid());
		printf("Real group-ID:      %d\n", getgid());
		printf("Effective user-ID:  %d\n", geteuid());
		printf("Effective group-ID: %d\n", getegid());
		printf("Saved user-ID:      %d\n", geteuid());
		printf("Saved group-ID:     %d\n", getegid());
		printf("\n");
	}
	
	// -----------------------------------------------------------------
	// Print the suppliementary group IDs
	{
		gid_t group_list[NGROUPS_MAX + 1];
		int group_cnt = getgroups(NGROUPS_MAX + 1, group_list);
		
		if (group_cnt == -1)
		{
			printf("ERROR: getgroups failure\n");
			return EXIT_FAILURE;
		}
		
		printf("Number of Supplementary Groups: %d\n", group_cnt);
		printf("List of Supplementary Groups (GID,name):\n");
		
		struct group* gr_info;
		int i;
		for (i = 0; i < group_cnt; ++i)
		{
			// Get the group information.
			gr_info = getgrgid(group_list[i]);
			
			// Print the group information.
			printf("- %4d, %s\n", gr_info->gr_gid, gr_info->gr_name);
		}
	}
}
