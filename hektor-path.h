#ifndef HEKTOR_PATH_H
#define HEKTOR_PATH_H

#include <errno.h>
#include <string.h>
#include <stdbool.h>

#include <basedir.h>
#include <libgen.h>
#include <sys/stat.h>

enum { MAX_PATH_LENGTH = 255 };

// Get the absolute path to snapshot storage.
bool path_make_storage_dir(char *path_buffer);

#endif
