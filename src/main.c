#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <linux/limits.h>

#include "tokenizer.h"
#include "version.h"
#include "command.h"

int main() {
	printf("%s", NAME_VER);
	setenv("PATH", "/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin", 1);
	for(;;) {
		// Print shell prompt
		char cwd[PATH_MAX];
		if (getcwd(cwd, sizeof(cwd)) == NULL) {
			strcpy(cwd, "cwd_failed");
		}
		printf("[%s: %s]# ", getlogin(), cwd);

		// Read user input
		char cmd_buffer[65535];
		if (fgets(cmd_buffer, sizeof(cmd_buffer), stdin) == NULL)
			continue;
		cmd_buffer[strcspn(cmd_buffer, "\n")] = 0;
		char** args = tokenize(cmd_buffer, " ", NULL);
		if (args == NULL) {
			perror(NAME_VER);
		}	

		// Execute command
		if (runcmd(args)) {
			perror(NAME_VER);
			free(args);
		}

	}
}
