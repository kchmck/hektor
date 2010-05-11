#include <stdbool.h>
#include <stdio.h>

#include "hektor-config.h"
#include "hektor-snapshots.h"

#include "hektor-error.h"

bool hektor_error_loading_config(config_t *const config) {
  printf("An error occured while loading hektor's configuration: '%s'.\n",
         lua_tostring(config->lua, -1));

  config_close(config);

  return false;
}

bool hektor_error_loading_plan(config_t *const config) {
  printf("An error occured while loading a usage plan from the configuration\n"
         "file. Note that usage_plan's value must be surrounded by quotes.\n");

  config_close(config);

  return false;
}

bool hektor_error_invalid_plan(const config_string_t plan_name) {
  printf("'%s' is an invalid plan (choose from home, pro, pro-plus, elite,\n"
         "elite-plus, or elite-premium.)\n", plan_name);

  return false;
}

bool hektor_error_loading_snapshots(const snapshots_t *const snapshots) {
  printf("An error occured while loading usage snapshots from '%s'.\n",
         snapshots->storage_path);

  return false;
}

bool hektor_error_saving_snapshots(const snapshots_t *const snapshots) {
  printf("An error occured while saving usage snapshots to '%s'.\n",
         snapshots->storage_path);

  return false;
}

bool hektor_error_invalid_command(const char *const command_name) {
  printf("'%s' is an invalid command.\n", command_name);

  return false;
}

bool hektor_error_with_modem(void) {
  printf("An error occured while requesting a page from the modem. Perhaps\n"
         "the network is down?\n");

  return false;
}
