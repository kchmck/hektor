#ifndef PATH_H
#define PATH_H

#include <stdbool.h>

enum { PATH_MAX_LENGTH = 256 + 1 };
typedef char path_t[PATH_MAX_LENGTH];

// Make a directory and all its parents.
bool make_dir(const path_t dir);

// Get the path to the config dir and config file.
bool get_config_paths(path_t dir_buffer, path_t file_buffer);

#endif
