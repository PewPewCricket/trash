#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <linux/limits.h>

#include "version.h"
#include "tokenizer.h"
#include "builtins.h"

int runcmd(char** args) {
	// Check if command is NULL
	if (args[0] == NULL)
		return 0;

	// Run built-ins
	int builtins_stat = check_builtins(args);
	if (builtins_stat == -1) {
		return -1;
	} else if (builtins_stat == 1) {
		return 0;
	}


	// Run command
	int status;
	pid_t cpid = fork();
	if (cpid == -1) {
	// If fork() fails:
		return -1;

	} else if (cpid == 0) {
	// In child process
		size_t pathdir_c;
		char** pathdirs = tokenize(getenv("PATH"), ":", &pathdir_c);
		if (pathdirs != NULL) {
			for (long unsigned int i = 0; i < pathdir_c; i++) {
				char cmd[PATH_MAX];
				snprintf(cmd, PATH_MAX, "%s/%s", pathdirs[i], args[0]);
				execve(cmd, (char *const*) args, 0);
			}
		}
		
		// If not in PATH dirs
		if (execve(args[0], (char *const*) args, 0) == -1)
			printf("%s: %s: %s\n", NAME, args[0], strerror(errno));
		exit(EXIT_FAILURE);

	} else {
	// In parent process
		pid_t wpid = waitpid(cpid, &status, WUNTRACED);
		if (wpid == -1) {
			return -1;
		}

		// Print status messages for finished processes.
		if (WIFEXITED(status)) {
			#ifdef DEBUG
				printf("Process finished (exit code %d)\n", WEXITSTATUS(status));
			#endif
		} else if (WIFSIGNALED(status)) {
			#ifdef DEBUG
				printf("Process killed (signal %d)\n", WTERMSIG(status));
			#endif
		} else if (WIFSTOPPED(status)) {
			#ifdef DEBUG
				printf("Process stopped (signal %d)\n", WSTOPSIG(status));
			#endif
		} else {
			printf("Unexpected status (0x%x)\n", status);
		}
		return 0;
	}
}
