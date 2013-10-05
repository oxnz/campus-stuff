// uid.m -- experiment with user and group ids.
// 		run as normal, then change this to be suid, and run again

#import <sys/types.h>	// for struct group/struct passwd
#import <grp.h>		// gor getgrgid
#import <pwd.h>		// for getpwuid
#import <stdio.h>
#import <stdlib.h>
#import <unistd.h>	// for getuid(), etc

int main (int argc, char *argv[]) {
	uid_t uid = getuid();
	uid_t euid = geteuid();

	gid_t gid = getgid();
	gid_t egid = getegid();

	struct passwd *user = getpwuid(uid);
	printf("real user ID is '%s'\n", user->pw_name);

	user = getpwuid(euid);
	printf("effective user ID is '%s'\n", user->pw_name);

	struct group *group = getgrgid(gid);
	printf("real group is '%s'\n", group->gr_name);

	group = getgrgid(egid);
	printf("effective group is '%s'\n", group->gr_name);

	return EXIT_SUCCESS;
} // main
