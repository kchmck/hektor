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

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <basedir.h>
#include <libgen.h>
#include <sys/stat.h>

#include "common.h"
#include "path.h"

// Make a single dir. Return true on success and false otherwise.
static bool make_dir(const path_t dir) {
  return mkdir(dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != -1 ||
         errno == EEXIST;
}

bool make_dir_and_parents(const path_t dir) {
  // This is required because dirname discards the constness of its argument.
  path_t dir_copy;
  string_copy(dir, dir_copy, PATH_MAX_LENGTH);

  const char *parent_dir = dirname(dir_copy);

  // If there are no more parents to create, then finish up...
  if (strings_are_equal(dir, parent_dir))
    return true;

  // ...else recursively make any parents...
  return make_dir_and_parents(parent_dir) && make_dir(dir);
}

// Build a path by appending @suffix onto @prefix.
static inline bool build_path(path_t path_buffer, const path_t prefix,
                              const path_t suffix)
{
  return snprintf(path_buffer, PATH_MAX_LENGTH, "%s/%s", prefix, suffix);
}

// Append the hektor subdirectory onto @dir.
static bool build_hektor_dir(path_t path_buffer, const path_t dir) {
  return build_path(path_buffer, dir, "hektor");
}

// Get the path to the config dir.
static bool build_config_dir(path_t path_buffer, xdgHandle *xdg_dirs) {
  return build_hektor_dir(path_buffer, xdgConfigHome(xdg_dirs));
}

// Get the path to the config file.
static bool build_config_file(path_t path_buffer, const path_t config_dir) {
  return build_path(path_buffer, config_dir, "config.lua");
}

bool get_config_paths(path_t dir_buffer, path_t file_buffer) {
  xdgHandle xdg_dirs;
  if (!xdgInitHandle(&xdg_dirs))
    return false;

  if (!build_config_dir(dir_buffer, &xdg_dirs))
    return false;

  xdgWipeHandle(&xdg_dirs);

  return build_config_file(file_buffer, dir_buffer);
}
