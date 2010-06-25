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

#ifndef PATH_H
#define PATH_H

#include <stdbool.h>

enum { PATH_MAX_LENGTH = 256 + 1 };
typedef char path_t[PATH_MAX_LENGTH];

// Make a @directory and all of its parents.
bool make_dir(const path_t dir);

// Get the paths to the config dir and the config file.
bool get_config_paths(path_t dir_buffer, path_t file_buffer);

#endif
