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

#ifndef HEKTOR_SNAPSHOTS_H
#define HEKTOR_SNAPSHOTS_H

#include <stdbool.h>

#include "hektor-path.h"
#include "hektor-snapshot.h"

enum { MAX_SNAPSHOTS = 250 };

typedef struct {
  snapshot_t list[MAX_SNAPSHOTS];
  int length;
  path_t storage_path;
} snapshots_t;

// Create a list of snapshots from a json file, and prune out any expired or
// corrupted ones.
bool snapshots_load(snapshots_t *const snapshots);

// Save a list of snapshots to the disk in json format.
bool snapshots_save(const snapshots_t *const snapshots);

// Get a new, empty snapshot at the end of the list.
snapshot_t *snapshots_get_next_empty(snapshots_t *const snapshots);

// Get the last recorded snapshot.
static inline snapshot_t *snapshots_get_last(snapshots_t *const snapshots) {
  return &snapshots->list[snapshots->length - 1];
}

// Get the first recorded snapshot.
static inline snapshot_t *snapshots_get_first(snapshots_t *const snapshots) {
  return &snapshots->list[0];
}

// Get a pair of snapshots.
bool snapshots_get_pair(const snapshots_t *const snapshots,
                        const snapshot_t **begin,
                        const snapshot_t **end,
                        const int snapshot_pair);

#endif
