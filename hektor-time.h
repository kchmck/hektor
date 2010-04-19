#ifndef HEKTOR_TIME_H
#define HEKTOR_TIME_H

#include <stdbool.h>
#include <time.h>

enum { MAX_TIME_FORMAT_LENGTH = 32 };

// Get the current local time.
static inline time_t now_local_time() { return time(NULL); }

// Replicate ctime without the static storage problems.
static inline bool format_time(const time_t *time, char *buffer) {
  return strftime(buffer, MAX_TIME_FORMAT_LENGTH, "%c", localtime(time));
}

#endif
