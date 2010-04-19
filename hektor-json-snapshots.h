#ifndef JSON_SNAPSHOTS_H
#define JSON_SNAPSHOTS_H

#include <jansson.h>
#include <stdio.h>

// Create a new, empty json snapshots array.
static inline json_t *json_snapshots_empty() { return json_array(); }

// Try to load snapshots from a json file.
json_t *json_snapshots_load(const char *filename);

// Save snapshots to a json file.
void json_snapshots_save(const json_t *json_snapshots, const char *filename);

#endif
