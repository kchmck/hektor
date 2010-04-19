#ifndef HEKTOR_SNAPSHOT_H
#define HEKTOR_SNAPSHOT_H

#include <stdbool.h>
#include <time.h>

#include <jansson.h>

#include "hektor-modem.h"
#include "hektor-pep.h"
#include "hektor-time.h"

typedef struct {
  time_t snapshot_time;
  int upload;
  int download;
} snapshot_t;

// Record a new snapshot.
bool snapshot_record(snapshot_t *snapshot, const char *pep_url);

// Load a snapshot from its json state.
bool snapshot_load(snapshot_t *snapshot, const json_t *json_snapshot);

// Save a snapshot into its json state and append it to the snapshots array.
bool snapshot_save(const snapshot_t *snapshot, json_t *snapshots);

// Check if a snapshot was recorded during the fap-free period (2am through 7am
// EST.)
// TODO: daylight savings time stuff?
bool snapshot_during_fap_free(const snapshot_t *snapshot);

#endif
