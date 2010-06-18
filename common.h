// Copyright 2010 Mick Koch <kchmck@gmail.com>
//
// This file is part of hektor.
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
// hektor. If not, see <http://www.gnu.org/licenses/>.

#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

// Find the minimum of two numbers.
static inline int min(const int a, const int b) { return a < b ? a : b; }

// Find the maximum of two numbers.
static inline int max(const int a, const int b) { return a > b ? a : b; }

// Check if two strings are the same.
static inline bool strings_are_equal(const char *a, const char *b) {
  return strcmp(a, b) == 0;
}

// Copy source to dest and ensure null termination.
static inline bool string_copy(const char *source, char *dest,
                               const size_t max_length)
{
  return snprintf(dest, max_length, "%s", source);
}

#endif
