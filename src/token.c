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

#include <stdlib.h>
#include <string.h>

char** tokenize(char* buf, char* delim, int* token_count) {
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
			for (size_t i = 0; i < count; i++) {
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