// Copyright 2010 Mick Koch <kchmck@gmail.com>
//
// This file is part of Hektor.
//
// Hektor is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// Hektor is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// Hektor. If not, see <http://www.gnu.org/licenses/>.

#ifndef HEKTOR_COMMON_H
#define HEKTOR_COMMON_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// Find the minimum of two numbers.
static inline int min(const int a, const int b) { return a < b ? a : b; }

// Find the maximum of two numbers.
static inline int max(const int a, const int b) { return a > b ? a : b; }

// Check if a string begins with a substring.
static inline bool string_begins_with(const char *const substring,
                                      const char *const string)
{
  return strstr(string, substring) == &string[0];
}

static inline bool strings_are_equal(const char *const a, const char *const b) {
  return strcmp(a, b) == 0;
}

// Copy source to dest and ensure null-termination.
static inline void string_copy(const char *const source, char *const dest,
                               const size_t max_length)
{
  snprintf(dest, max_length, "%s", source);
}

#endif
