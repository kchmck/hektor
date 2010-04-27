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

#include "hektor-snapshots.h"

bool snapshots_load(snapshots_t *snapshots) {
  // The fap is calculated on a rolling 24-hour time span, so any snapshots
  // older than 24 hours are expired.
  enum { EXPIRE_SNAPSHOTS_AFTER = 24 * 60 * 60 };

  // Find (and create if necessary) the path to the json snapshots.
  if (!path_make_storage_dir(snapshots->json_storage_path)) return false;

  // Try to load the snapshots.
  json_t *json_snapshots = json_snapshots_load(snapshots->json_storage_path);
  if (!json_snapshots) return false;

  const int array_size = json_array_size(json_snapshots);

  // If there are no json snapshots, skip what follows.
  if (!array_size) goto finished;

  const time_t expire_point = now_local_time() - EXPIRE_SNAPSHOTS_AFTER;
  int json_index = 0;

  while (snapshots->length < MAX_SNAPSHOTS) {
    const json_t *json_snapshot = json_array_get(json_snapshots, json_index++);
    if (!json_snapshot) break;

    // Try to load the snapshot (if it isn't corrupted.)
    snapshot_t *current_snapshot = &snapshots->list[snapshots->length];
    if (!snapshot_load(current_snapshot, json_snapshot)) continue;

    // Check if the snapshot is expired.
    if (current_snapshot->snapshot_time < expire_point) continue;

    ++snapshots->length;
  }

finished:
  json_decref(json_snapshots);

  return true;
}

bool snapshots_save(const snapshots_t *snapshots) {
  json_t *json_snapshots = json_snapshots_empty();
  if (!json_snapshots) return false;

  for (int i = 0; i < snapshots->length; ++i)
    snapshot_save(&snapshots->list[i], json_snapshots);

  json_snapshots_save(json_snapshots, snapshots->json_storage_path);
  json_decref(json_snapshots);

  return true;
}

snapshot_t *snapshots_next_empty(snapshots_t *snapshots) {
  if (snapshots->length < MAX_SNAPSHOTS)
    return &snapshots->list[snapshots->length++];

  // If the list is completely full, chop a snapshot off the beginning in order
  // to make room for a new one at the end.
  memmove(&snapshots->list[0], &snapshots->list[1], sizeof(snapshots->list));

  // Return the absolute last snapshot.
  return &snapshots->list[snapshots->length - 1];
}

bool snapshots_get_pair(const snapshots_t *snapshots, const snapshot_t **begin,
                        const snapshot_t **end, const int snapshot_pair)
{
  if (snapshot_pair + 1 >= snapshots->length) return false;

  *begin = &snapshots->list[snapshot_pair];
  *end = &snapshots->list[snapshot_pair + 1];

  return true;
}
