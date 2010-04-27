#ifndef HEKTOR_ERROR_H
#define HEKTOR_ERROR_H

#include <stdbool.h>
#include <stdio.h>

#include "hektor-config.h"
#include "hektor-snapshots.h"

static inline bool hektor_error_loading_config(config_t *config) {
  printf("An error occured while loading hektor's configuration: '%s'.\n",
         lua_tostring(config->lua, -1));
  config_close(config);

  return false;
}

static inline bool hektor_error_loading_plan(config_t *config) {
  printf("An error occured while loading a usage plan from the configuration\n"
         "file. Note that the usage_plan option must be a string.\n");
  config_close(config);

  return false;
}

static inline bool hektor_error_invalid_plan(config_string_t plan_name) {
  printf("'%s' is an invalid plan (choose from home, pro, pro-plus, elite,\n"
         "elite-plus, or elite-premium.)\n", plan_name);

  return false;
}

static inline bool hektor_error_loading_snapshots(snapshots_t *snapshots) {
  printf("An error occured while loading usage snapshots from '%s'.\n",
         snapshots->storage_path);

  return false;
}

static inline bool hektor_error_saving_snapshots(snapshots_t *snapshots) {
  printf("An error occured while saving usage snapshots to '%s'.\n",
         snapshots->storage_path);

  return false;
}

static inline bool hektor_error_invalid_command(const char *command_name) {
  printf("'%s' is an invalid command.\n", command_name);
  return false;
}

#endif
