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
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along with
// Hektor.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEKTOR_TIME_H
#define HEKTOR_TIME_H

#include <stdbool.h>
#include <time.h>

enum { MAX_TIME_FORMAT_LENGTH = 32 };

// Get the current local time.
static inline time_t now_local_time() { return time(NULL); }

// Replicate ctime without the static storage problems.
static inline bool format_time(const time_t *time, char *buffer) {
  return strftime(buffer, MAX_TIME_FORMAT_LENGTH, "%c", localtime(time));
}

#endif
