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

#include <stdbool.h>
#include <time.h>

#include <jansson.h>

#include "hektor-modem.h"
#include "hektor-pep.h"
#include "hektor-time.h"

#include "hektor-snapshot.h"

void snapshot_record(snapshot_t *const snapshot, const page_t pep_page) {
  snapshot->snapshot_time = now_local_time();
  snapshot->upload = pep_extract_value("tx_bytes", pep_page);
  snapshot->download = pep_extract_value("rx_bytes", pep_page);
}

bool snapshot_load(snapshot_t *const snapshot,
                   const json_t *const json_snapshot) 
{
  if (!json_is_object(json_snapshot)) return false;

  const json_t *const snapshot_time = json_object_get(json_snapshot, "time");
  const json_t *const upload = json_object_get(json_snapshot, "upload");
  const json_t *const download = json_object_get(json_snapshot, "download");

  if (!snapshot_time || !upload || !download) return false;

  snapshot->snapshot_time = json_real_value(snapshot_time);
  snapshot->upload = json_real_value(upload);
  snapshot->download = json_real_value(download);

  return true;
}

bool snapshot_save(const snapshot_t *const snapshot, json_t *const snapshots) {
  json_t *const snapshot_object = json_object();
  if (!snapshot_object) return false;

  json_t *const snapshot_time = json_real(snapshot->snapshot_time);
  json_t *const upload = json_real(snapshot->upload);
  json_t *const download = json_real(snapshot->download);

  if (!snapshot_time || !upload || !download) goto error;

  if (json_object_set_new(snapshot_object, "time", snapshot_time) == -1
  ||  json_object_set_new(snapshot_object, "upload", upload) == -1
  ||  json_object_set_new(snapshot_object, "download", download) == -1)
    goto error;

  return json_array_append_new(snapshots, snapshot_object) != -1;

error:
  json_decref(snapshot_time);
  json_decref(upload);
  json_decref(download);

  return false;
}

bool snapshot_during_fap_free(const snapshot_t *const snapshot) {
  // 2am and 7am EST in UTC
  enum { BEGIN_UTC = 6, END_UTC = 11 };

  // Get the snapshot time info in UTC.
  const struct tm *const time_info = gmtime(&snapshot->snapshot_time);

  return time_info->tm_hour >= BEGIN_UTC && time_info->tm_hour < END_UTC;
}
