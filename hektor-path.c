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

#include "hektor-path.h"

// Create a new directory and all its parents.
static bool path_make_dirs(const path_t full_path) {
  enum { MODE_BITS = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH };

  // This is required because dirname discards the constness of its argument.
  path_t full_path_copy = {0};
  memcpy(full_path_copy, full_path, MAX_PATH_LENGTH);

  const char *parent_dir = dirname(full_path_copy);
  // If there are no more parents to create, then finish up...
  if (strcmp(full_path, parent_dir) == 0) return true;

  // ...else recursively make any parents.
  if (!path_make_dirs(parent_dir)) return false;
  if (mkdir(full_path, MODE_BITS) == -1 && errno != EEXIST) return false;

  return true;
}

bool path_make_storage_dir(path_t path_buffer) {
  xdgHandle freedesktop_dirs;
  if (!xdgInitHandle(&freedesktop_dirs)) return false;

  // Get the data dir.
  strncat(path_buffer, xdgDataHome(&freedesktop_dirs), MAX_PATH_LENGTH);
  xdgWipeHandle(&freedesktop_dirs);

  // Add hektor's subdir and try to create it.
  strncat(path_buffer, "/hektor/", MAX_PATH_LENGTH - strlen(path_buffer));
  if (!path_make_dirs(path_buffer)) return false;

  // Add the snapshots' filename.
  strncat(path_buffer, "snapshots.json", MAX_PATH_LENGTH - strlen(path_buffer));

  return true;
}
