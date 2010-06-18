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
#include <stdlib.h>
#include <time.h>

#include "config.h"
#include "fap.h"
#include "hook.h"
#include "lua-util.h"
#include "modem.h"
#include "unit.h"

typedef struct {
  fap_t fap;

  lua_t lua;
  config_t config;

  hook_t remaining_hook;
  hook_t refill_hook;
} hektor_t;

static void hektor_handle_refill(hektor_t *hektor) {
  const time_t refill_time = fap_refill_time(&hektor->fap);
  const time_t refill_timestamp = fap_refill_timestamp(&hektor->fap);

  unit_t refill_time_unit;
  unit_convert_best(&refill_time_unit, refill_time, UNIT_SECOND);

  hook_call(&hektor->refill_hook, "lls", refill_time, refill_timestamp,
                                         unit_string(&refill_time_unit));
}

static void hektor_handle_remaining(hektor_t *hektor) {
  const double remaining_mb = unit_convert(fap_remaining_usage(&hektor->fap),
                                           UNIT_BYTE, UNIT_MEGABYTE);

  unit_t remaining_unit;
  unit_convert_best(&remaining_unit, fap_remaining_usage(&hektor->fap),
                    UNIT_BYTE);

  hook_call(&hektor->remaining_hook, "fs", remaining_mb,
                                           unit_string(&remaining_unit));
}

static bool hektor_error_fetching_page(const url_t url) {
  printf("An error occured while fetching the page at ‘%s’.\n", url);

  return false;
}

static bool hektor_error_loading_config(hektor_t *hektor) {
  printf("An error occured while load the configuration file \n"
         "at ‘%s’: %s.\n",
         
         config_file_path(&hektor->config),
         lua_tostring(lua_state(&hektor->lua), -1));

  return false;
}

static bool hektor_main(hektor_t *hektor) {
  url_t info_url;
  if (!modem_get_info_url(info_url))
    return false;

  page_t info_page;
  if (!modem_fetch_page(info_page, info_url))
    return hektor_error_fetching_page(info_url);

  fap_init(&hektor->fap, info_page);

  if (!config_init(&hektor->config, &hektor->lua))
    return false;

  // Set up the remaining hook.
  hook_init(&hektor->remaining_hook, &hektor->lua);
  hook_register(&hektor->remaining_hook, "hook_remaining");

  // Set up the refill hook.
  hook_init(&hektor->refill_hook, &hektor->lua);
  hook_register(&hektor->refill_hook, "hook_refill");

  if (!config_load(&hektor->config))
    return hektor_error_loading_config(hektor);

  if (fap_is_active(&hektor->fap))
    hektor_handle_refill(hektor);
  else
    hektor_handle_remaining(hektor);

  return true;
}

static bool hektor_main_wrapper(int argc, char **argv) {
  hektor_t hektor;

  if (!lua_init(&hektor.lua))
    return false;

  modem_global_init();

  const bool result = hektor_main(&hektor);

  modem_global_destroy();
  lua_destroy(&hektor.lua);

  return result;
}

int main(int argc, char **argv) {
  return hektor_main_wrapper(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
