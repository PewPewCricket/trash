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

#ifndef USAGE_H
#define USAGE_H 1

#include <version.h>

char* usage_text =
  "trash: basic linux shell program\n"
  "Usage:\n"
  "\ttrash [options]\n"
  "Options:\n"
  "\t--help:        Print this help message.\n"
  "\t--version:     Print the current version.\n";

char* version_text = 
  "Trash, version " PROGRAM_VERSION_STRING "\n"
  "Copyright (C) 2025 PewPewCricket\n"
  "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n\n"
  "This is free software; you are free to change and redistribute it.\n"
  "There is NO WARRANTY, to the extent permitted by law.\n";

#endif