#include "hektor-path.h"

// Create a new directory and all its parents.
static bool path_make_dirs(const char *full_path) {
  enum { MODE_BITS = S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH };

  // This is required because dirname discards the constness of its argument.
  char full_path_copy[MAX_PATH_LENGTH] = {0};
  memcpy(full_path_copy, full_path, MAX_PATH_LENGTH);

  const char *parent_dir = dirname(full_path_copy);
  // If there are no more parents to create, then finish up...
  if (strcmp(full_path, parent_dir) == 0) return true;

  // ...else recursively make any parents.
  if (!path_make_dirs(parent_dir)) return false;
  if (mkdir(full_path, MODE_BITS) == -1 && errno != EEXIST) return false;

  return true;
}

bool path_make_storage_dir(char *path_buffer) {
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
