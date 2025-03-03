#include <stdlib.h>
#include <string.h>

char** tokenize(char* buf, char* delim) {
	// Copy string, strtok() is destructive so we don't want to destroy the original string.
	char* s = strdup(buf);
	if (s == NULL) {
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
		return NULL;
	}

	// Create our argument array.
	char** tokens = (char**)malloc((count + 1) * sizeof(char*));
	if (tokens == NULL) {
		free(s);
		return NULL;
	}

	// Tokenize string.
	count = 0;
	token = strtok(s, delim);
	while (token != NULL) {
		tokens[count] = strdup(token);
		if (tokens[count] == NULL) {
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
	free(s);
	return tokens;
}
