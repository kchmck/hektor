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

#include <jansson.h>
#include <stdbool.h>
#include <stdio.h>

#include "hektor-json-snapshots.h"
#include "hektor-path.h"

json_t *json_snapshots_load(const path_t filename) {
  FILE *snapshots_file = fopen(filename, "r");
  if (!snapshots_file) return json_snapshots_empty();

  json_error_t error;
  json_t *json_snapshots = json_loadf(snapshots_file, &error);

  fclose(snapshots_file);

  if (!json_snapshots) return json_snapshots_empty();
  if (json_is_array(json_snapshots)) return json_snapshots;

  json_decref(json_snapshots);

  return json_snapshots_empty();
}

bool json_snapshots_save(const json_t *json_snapshots, const path_t filename) {
  FILE *snapshots_file = fopen(filename, "w");
  if (!snapshots_file) return false;

  json_dumpf(json_snapshots, snapshots_file, JSON_COMPACT);
  fclose(snapshots_file);

  return true;
}
