#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <basedir.h>
#include <libgen.h>
#include <sys/stat.h>

#include "common.h"
#include "path.h"

bool make_dir(const path_t dir) {
  enum { MODE_BITS = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH };

  // This is required because dirname discards the constness of its argument.
  path_t dir_copy;
  string_copy(dir, dir_copy, PATH_MAX_LENGTH);

  const char *parent_dir = dirname(dir_copy);

  // If there are no more parents to create, then finish up...
  if (strings_are_equal(dir, parent_dir))
    return true;

  // ...else recursively make any parents.
  if (!make_dir(parent_dir))
    return false;

  if (mkdir(dir, MODE_BITS) == -1 && errno != EEXIST)
    return false;

  return true;
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

bool get_config_paths(path_t dir_buffer, path_t file_buffer) {
  xdgHandle xdg_dirs;
  if (!xdgInitHandle(&xdg_dirs))
    return false;

  if (!build_config_dir(dir_buffer, &xdg_dirs))
    return false;

  xdgWipeHandle(&xdg_dirs);

  return build_path(file_buffer, dir_buffer, "config.lua");
}
