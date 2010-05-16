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

#ifndef HEKTOR_SNAPSHOT_H
#define HEKTOR_SNAPSHOT_H

#include <stdbool.h>
#include <time.h>

#include <jansson.h>

#include "hektor-modem.h"

typedef struct {
  time_t snapshot_time;
  long upload;
  long download;
} snapshot_t;

// Record a new snapshot.
void snapshot_record(snapshot_t *snapshot, const page_t pep_page);

// Load a snapshot from its json state.
bool snapshot_load(snapshot_t *snapshot,
                   const json_t *json_snapshot);

// Save a snapshot into its json state and append it to the snapshots array.
bool snapshot_save(const snapshot_t *snapshot, json_t *snapshots);

// Check if a snapshot was recorded during the fap-free period (2am through 7am
// EST.)
// TODO: daylight savings time stuff?
bool snapshot_during_fap_free(const snapshot_t *snapshot);

#endif
