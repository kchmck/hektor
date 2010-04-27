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

#ifndef HEKTOR_PATH_H
#define HEKTOR_PATH_H

#include <errno.h>
#include <string.h>
#include <stdbool.h>

#include <basedir.h>
#include <libgen.h>
#include <sys/stat.h>

enum { MAX_PATH_LENGTH = 255 };

// A path type
typedef char path_t[MAX_PATH_LENGTH];

// Get the absolute path to snapshot storage.
bool path_make_storage_dir(path_t path_buffer);

#endif
