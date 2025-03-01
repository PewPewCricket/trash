#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <linux/limits.h>

#define NAME "shell"

char** tokenize(char* buf, char* delim, int* token_count);

int main() {
	for(;;) {
		// Print shell prompt
		char cwd[PATH_MAX];
		if (getcwd(cwd, sizeof(cwd)) == NULL) {
			strcpy(cwd, "cwd_failed");
		}
		printf("[%s: %s]# ", getlogin(), cwd);

		// Read user input
		char cmd_buffer[999];
		if (fgets(cmd_buffer, sizeof(cmd_buffer), stdin) == NULL)
			continue;
		cmd_buffer[strcspn(cmd_buffer, "\n")] = 0;
		char** args = tokenize(cmd_buffer, " ", NULL);

		// Execute built-in commands
		if (strcmp(args[0], "cd") == 0) {
			if (chdir(args[1]) == -1) {
				perror(NAME);
			}
			continue;
		} else if (strcmp(args[0], "clear") == 0) {
			printf("\e[1;1H\e[2J");
			continue;
		}

		// Execute new process
		int status;
		pid_t cpid = fork();
		if (cpid == -1) {
			// If fork() fails:
			perror(NAME);
			continue;

		} else if (cpid == 0) {
			// In child process
			if (execve(args[0], (char *const*) args, 0) == -1) {
				perror(NAME);
			}
			exit(EXIT_FAILURE);

		} else {
			// In parent process
			pid_t wpid = waitpid(cpid, &status, WUNTRACED);
			if (wpid == -1) {
				perror(NAME);
				continue;
			}

			// Print status messages for finished processes.
			if (WIFEXITED(status)) {
				//printf("Process finished (exit code %d)\n", WEXITSTATUS(status));
			} else if (WIFSIGNALED(status)) {
				//printf("Process killed (signal %d)\n", WTERMSIG(status));
			} else if (WIFSTOPPED(status)) {
				//printf("Process stopped (signal %d)\n", WSTOPSIG(status));
			} else {
				printf("Unexpected status (0x%x)\n", status);
			}	
		}
	}
}

char** tokenize(char* buf, char* delim, int* token_count) {
	// Copy string, strtok() is destructive so we don't want to destroy the original string.
	char* s = strdup(buf);
	if (s == NULL) {
		perror(NAME);
		return NULL;
	}
	
	// Get number of tokens
	int count = 0;
	char* token = strtok(s, delim);
	while (token != NULL) {
		count++;
		token = strtok(NULL, delim);
	}

	// Our copied string has been destroyed, it must be re-created.
	free(s);
	s = strdup(buf);
	if (s == NULL) {
		perror(NAME);
		return NULL;
	}

	// Create our argument array.
	char** tokens = (char**)malloc((count + 1) * sizeof(char*));
	if (tokens == NULL) {
		perror(NAME);
		free(s);
		return NULL;
	}

	// Tokenize string.
	count = 0;
	token = strtok(s, delim);
	while (token != NULL) {
		tokens[count] = strdup(token);
		if (tokens[count] == NULL) {
			perror(NAME);
			// Free all previously allocated tokens.
			for (int i = 0; i < count; i++) {
				free(tokens[i]);
			}
			free(tokens);
			free(s);
			return NULL;
		}
		count++;
		token = strtok(NULL, delim);
	}

	// Clean up.
	tokens[count] = NULL;
	if (token_count != NULL)
		*token_count = count;
	free(s);
	return tokens;
}
