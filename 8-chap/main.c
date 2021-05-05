#include <pwd.h>
#include <stdio.h>


int main(int argc, char* argv[])
{
	// ---------------
	// Print the  user information.
	// ---------------
	{
		struct passwd* pwd = getpwnam("pi");
		printf("Name:        %s\n", pwd->pw_name);
		printf("Password:    %s\n", pwd->pw_passwd);
		printf("User-ID:     %d\n", pwd->pw_uid);
		printf("Group-ID:    %d\n", pwd->pw_gid);
		printf("Comments:    %s\n", pwd->pw_gecos);
		printf("Home Dir:    %s\n", pwd->pw_dir);
		printf("Login Shell: %s\n", pwd->pw_shell);
	}
	
	
	
	return 0;
}
