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
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along with
// Hektor.  If not, see <http://www.gnu.org/licenses/>.

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
