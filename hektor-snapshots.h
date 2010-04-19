#ifndef HEKTOR_SNAPSHOTS_H
#define HEKTOR_SNAPSHOTS_H

#include <stdbool.h>
#include <jansson.h>

#include "hektor-json-snapshots.h"
#include "hektor-path.h"
#include "hektor-snapshot.h"
#include "hektor-time.h"

enum { MAX_SNAPSHOTS = 250 };

typedef struct {
  snapshot_t list[MAX_SNAPSHOTS];
  int length;
  char json_storage_path[MAX_PATH_LENGTH];
} snapshots_t;

// Create a list of snapshots from a json file, and prune out any expired or
// corrupted ones.
bool snapshots_load(snapshots_t *snapshots);

// Save a list of snapshots to the disk in json format.
bool snapshots_save(const snapshots_t *snapshots);

// Get a new, empty snapshot at the end of the list.
snapshot_t *snapshots_next(snapshots_t *snapshots);

#endif
