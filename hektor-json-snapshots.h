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

#ifndef JSON_SNAPSHOTS_H
#define JSON_SNAPSHOTS_H

#include <jansson.h>
#include <stdbool.h>

#include "hektor-path.h"

// Create a new, empty json snapshots array.
static inline json_t *json_snapshots_empty() { return json_array(); }

// Load snapshots from a json file.
json_t *json_snapshots_load(const path_t filename);

// Save snapshots to a json file.
bool json_snapshots_save(const json_t *const json_snapshots,
                         const path_t filename);

#endif
