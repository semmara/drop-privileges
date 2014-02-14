#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <stdlib.h>
#include <errno.h>
#include <grp.h>

int
main(int argc, char **argv) {
	struct passwd *pwd;
	int ret = 0;

	/* usage */
	if (argc != 3) {
		fprintf(stderr, "Usage: %s username \"command\"\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	/* get user data */
	if (!(pwd = getpwnam(argv[1]))) {
		printf("error: getpwnam failed\n");
	}
#ifdef DEBUG
	printf( "username = %s\n"
		"password = %s\n"
		"euid = %d\n"
		"egid = %d\n"
		"user info = %s\n"
		"home dir = %s\n"
		"shell program = %s\n",
		pwd->pw_name,
		pwd->pw_passwd,
		pwd->pw_uid,
		pwd->pw_gid,
		pwd->pw_gecos,
		pwd->pw_dir,
		pwd->pw_shell);
#endif

	/* set user privileges */
	if (seteuid(pwd->pw_uid) != 0) {
		printf("error: seteuid failed\n");
		exit(EXIT_FAILURE);
	}
#ifdef DEBUG
	if (setegid(pwd->pw_gid) != 0) {
		printf("error: setegid failed\n");
		exit(EXIT_FAILURE);
	}
#endif

	/* run command */
	if ((ret = system(argv[2])) != 0) {
		printf("error: system failed\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}
