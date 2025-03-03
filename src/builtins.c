#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "tokenizer.h"

char* trim_spaces(char* s) {
	char* str = strdup(s);
	int i = 0, j = 0;
	while (str[i]) {
		if (!isspace(str[i]))
			str[j++] = str[i];
		i++;
	}
	str[j] = '\0';
	return str;
}

int check_builtins(char** args) {
	if (strcmp(args[0], "cd") == 0) {
		if (chdir(args[1]) == -1) {
			return -1;
		}
		return 1;

	} else if (strcmp(args[0], "clear") == 0) {
		printf("\e[1;1H\e[2J");
		return 1;

	} else if (strcmp(args[0], "exit") == 0) {
		exit(EXIT_SUCCESS);

	} else if (strchr(args[0], '=') != NULL) {
		char** tokens = tokenize(trim_spaces(args[0]), "=", NULL);
		if (tokens == NULL)
			return -1;
		setenv(tokens[0], tokens[1], true);
		return 1;

	} else {
		return 0;
	}
}
