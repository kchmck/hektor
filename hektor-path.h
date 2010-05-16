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

#ifndef HEKTOR_PATH_H
#define HEKTOR_PATH_H

#include <stdbool.h>

// A path type
enum { MAX_PATH_LENGTH = 255 };
typedef char path_t[MAX_PATH_LENGTH];

// Find the path to snapshots storage, create any needed dirs, and write the
// absolute path into path_buffer.
bool path_make_snapshots_storage(path_t path_buffer);

// Same as above, exept for config storage.
bool path_make_config_storage(path_t path_buffer);

#endif
