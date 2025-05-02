/* trash: basic linux shell program.
 * Copyright (C) 2025  PewPewCricket
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <linux/limits.h>

#include <version.h>
#include <usage.h>
#include <token.h>

extern char **environ;

static struct option long_opts[] = {
  {"help",      no_argument, 0, 0},
  {"version",   no_argument, 0, 0},
  {0,           0,           0, 0}
};

int main(int argc, char** argv) {
  /* Option and argument parsing */
  int opt;

  for (;;) {
    int opt_idx = 0;
    opt = getopt_long(argc, argv, "", long_opts, &opt_idx);

    if (opt == -1) break;
    switch (opt) {
      case 0:
        if (!strcmp(long_opts[opt_idx].name, "help")) {
          printf("%s", usage_text);
          return EXIT_SUCCESS;
        } else if (!strcmp(long_opts[opt_idx].name, "version")) {
          printf("%s", version_text);
          return EXIT_SUCCESS;
        }
        break;
      case '?': /* getopt_long already printed an error message. */
        break;
      default:
        abort();
    }
  }

  /* if PATH is not set then set to to the default */
  if (setenv("PATH", "/bin:/sbin:/usr/bin:/usr/sbin:usr/local/bin:/usr/local/sbin", 0) != 0) {
    perror(PROGRAM_NAME_STRING);
    printf("%s: $PATH is not set!\n", PROGRAM_NAME_STRING);
  }

  /* shell main loop */
  for (;;) {
    /* Print shell prompt */
    char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
      perror(PROGRAM_NAME_STRING);
      return EXIT_FAILURE;
    }

    printf("[%s: %s]# ", getlogin(), cwd);

    /* Get user input */
    char* line = NULL;
    size_t len = 0;
    ssize_t nread;

    nread = getline(&line, &len, stdin);

    if (nread == -1) {
      perror(PROGRAM_NAME_STRING);
      free(line);
      return EXIT_FAILURE;
    } else if (nread < 2) {
      free(line);
      continue;
    }

    line[strcspn(line, "\n")] = '\0';
    
    int token_c;
    char** tokens = tokenize(line, " ", &token_c);
    if (tokens == NULL) {
      perror(PROGRAM_NAME_STRING);
      return EXIT_FAILURE;
    }

    /* Execute command */
    if (!strcmp(tokens[0], "cd")) {
      if (token_c > 1) {
        if (chdir(tokens[1]) != 0)
          perror(PROGRAM_NAME_STRING);
      }
      free(tokens);
      free(line);
      continue;

    } else if (!strcmp(tokens[0], "exit")) {
      free(tokens);
      free(line);
      return EXIT_SUCCESS;

    } else if (!strcmp(tokens[0], "export")) {
      if (tokens[1] == NULL) {
        char **env = environ;
        while (*env) {
          printf("\t%s\n", *env);
          env++;
        }

        free(tokens);
        free(line);
        continue;
      }

      int pos = (int) (strstr(tokens[1], "=") - tokens[1]);
      if (pos == 0) {
        printf("%s: invalid syntax\n", PROGRAM_NAME_STRING);
      } else {
        char* env_var = malloc(pos);
        memcpy(env_var, tokens[1], pos);
        if (setenv(env_var, tokens[1] + pos, 1) != 0)
          perror(PROGRAM_NAME_STRING);
        free(env_var);
      }

      free(tokens);
      free(line);
      continue;
    }

    int status;
	  pid_t cpid = fork();

    if (cpid == -1) {         /* fork() failure */
      perror(PROGRAM_NAME_STRING);
      return -1;

    } else if (cpid == 0) {   /* Child process */
       /* Should never return */
      execvp(tokens[0], tokens);

      /* If we get here execvp failed.*/
      printf("%s: %s: %s\n", PROGRAM_NAME_STRING, tokens[0], strerror(errno));
      exit(EXIT_FAILURE);

    } else {                  /* Parent process */
      pid_t wpid = waitpid(cpid, &status, WUNTRACED);
      if (wpid == -1) {
        free(tokens);
        free(line);
        continue;
      }
    }

    /* cleanup */
    free(tokens);
    free(line);
  }
}