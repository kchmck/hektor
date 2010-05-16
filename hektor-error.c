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

#include <stdbool.h>
#include <stdio.h>

#include "hektor-config.h"
#include "hektor-snapshots.h"

#include "hektor-error.h"

bool hektor_error_loading_config(config_t *config) {
  printf("An error occured while loading hektor's configuration: '%s'.\n",
         lua_tostring(config->lua, -1));

  config_close(config);

  return false;
}

bool hektor_error_loading_plan(config_t *config) {
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

bool hektor_error_loading_snapshots(const snapshots_t *snapshots) {
  printf("An error occured while loading usage snapshots from '%s'.\n",
         snapshots->storage_path);

  return false;
}

bool hektor_error_saving_snapshots(const snapshots_t *snapshots) {
  printf("An error occured while saving usage snapshots to '%s'.\n",
         snapshots->storage_path);

  return false;
}

bool hektor_error_invalid_command(const char *command_name) {
  printf("'%s' is an invalid command.\n", command_name);

  return false;
}

bool hektor_error_with_modem(void) {
  printf("An error occured while requesting a page from the modem. Perhaps\n"
         "the network is down?\n");

  return false;
}
