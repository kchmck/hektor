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

bool path_dirname(const path_t path, path_t dirname_buffer) {
  // Required for libgen
  path_t path_copy;

  return string_copy(path, path_copy, PATH_LENGTH) &&
         string_copy(dirname(path_copy), dirname_buffer, PATH_LENGTH);
}

// Make a single @dir.
static bool path_make_dir(const path_t dir) {
  return mkdir(dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != -1 ||
         errno == EEXIST;
}

bool path_make_dirs(const path_t dir) {
  path_t parent_dirs;
  
  if (!path_dirname(dir, parent_dirs))
    return false;

  // If there are no more parents to create, then finish up...
  if (strings_are_equal(dir, parent_dirs))
    return true;

  // ...else recursively make any parents...
  return path_make_dirs(parent_dirs) && path_make_dir(dir);
}

// Build a path by appending @suffix onto @prefix.
static inline bool path_build(path_t path_buffer, const path_t prefix,
                                                  const path_t suffix)
{
  return snprintf(path_buffer, PATH_LENGTH, "%s/%s", prefix, suffix) > 0;
}

// Append the hektor subdirectory onto @dir.
static bool path_build_hektor_dir(path_t path_buffer, const path_t dir) {
  return path_build(path_buffer, dir, "hektor");
}

// Get the path to the config dir.
static bool path_build_config_dir(path_t path_buffer, xdgHandle *xdg_dirs) {
  return path_build_hektor_dir(path_buffer, xdgConfigHome(xdg_dirs));
}

// Get the path to the config file.
static bool path_build_config_file(path_t path_buffer, const path_t config_dir)
{
  return path_build(path_buffer, config_dir, "config.lua");
}

bool path_build_config(path_t dir_buffer, path_t file_buffer) {
  xdgHandle xdg_dirs;

  if (!xdgInitHandle(&xdg_dirs))
    return false;

  if (!path_build_config_dir(dir_buffer, &xdg_dirs)) {
    xdgWipeHandle(&xdg_dirs);

    return false;
  }

  xdgWipeHandle(&xdg_dirs);

  return path_build_config_file(file_buffer, dir_buffer);
}
