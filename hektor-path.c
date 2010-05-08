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

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <basedir.h>
#include <libgen.h>
#include <sys/stat.h>

#include "hektor-path.h"

// Create a new directory and any of its parents, if necessary.
static bool make_dir_and_parents(const path_t full_path) {
  enum { MODE_BITS = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH };

  // This is required because dirname discards the constness of its argument.
  path_t full_path_copy;
  strncpy(full_path_copy, full_path, MAX_PATH_LENGTH);

  const char *parent_dir = dirname(full_path_copy);
  // If there are no more parents to create, then finish up...
  if (strcmp(full_path, parent_dir) == 0) return true;

  // ...else recursively make any parents.
  if (!make_dir_and_parents(parent_dir)) return false;
  if (mkdir(full_path, MODE_BITS) == -1 && errno != EEXIST) return false;

  return true;
}

typedef bool (*xdg_dir_fn_t)(path_t, xdgHandle *);

// Append the hektor subdir onto a path.
static inline bool xdg_hektor_dir(path_t path_buffer, const char *xdg_dir) {
  return snprintf(path_buffer, MAX_PATH_LENGTH, "%s/hektor", xdg_dir);
}

// Find the path to hektor's general data storage dir.
static inline bool xdg_data_dir(path_t path_buffer, xdgHandle *xdg_dirs) {
  return xdg_hektor_dir(path_buffer, xdgDataHome(xdg_dirs));
}

// Same as above, but for the config dir.
static inline bool xdg_config_dir(path_t path_buffer, xdgHandle *xdg_dirs) {
  return xdg_hektor_dir(path_buffer, xdgConfigHome(xdg_dirs));
}

// Use fn to get a dir's path, then create it.
static bool make_xdg_dir(xdg_dir_fn_t fn, path_t path_buffer) {
  xdgHandle xdg_dirs;
  if (!xdgInitHandle(&xdg_dirs)) return false;

  const bool success = fn(path_buffer, &xdg_dirs);

  xdgWipeHandle(&xdg_dirs);

  return success && make_dir_and_parents(path_buffer);
}

// Same as above, and also create any needed dirs.
static inline bool make_data_dir(path_t path_buffer) {
  return make_xdg_dir(xdg_data_dir, path_buffer);
}

// Same as make_data_dir, but for the config dir.
static inline bool make_config_dir(path_t path_buffer) {
  return make_xdg_dir(xdg_config_dir, path_buffer);
}

bool path_make_snapshots_storage(path_t path_buffer) {
  path_t data_dir;
  if (!make_data_dir(data_dir)) return false;

  return snprintf(path_buffer, MAX_PATH_LENGTH, "%s/snapshots.json", data_dir);
}

bool path_make_config_storage(path_t path_buffer) {
  path_t config_dir;
  if (!make_config_dir(config_dir)) return false;

  return snprintf(path_buffer, MAX_PATH_LENGTH, "%s/config.lua", config_dir);
}
