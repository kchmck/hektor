#include "hektor-json-snapshots.h"

json_t *json_snapshots_load(const char *filename) {
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

void json_snapshots_save(const json_t *json_snapshots, const char *filename) {
  FILE *snapshots_file = fopen(filename, "w");

  json_dumpf(json_snapshots, snapshots_file, JSON_COMPACT);
  fclose(snapshots_file);
}
