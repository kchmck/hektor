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

#include <lua.h>

#include "config.h"
#include "hook.h"
#include "info.h"
#include "lua-util.h"
#include "modem.h"
#include "unit.h"

typedef struct {
  info_t info;

  lua_t lua;
  config_t config;

  hook_t fap_is_active_hook;
  hook_t fap_is_inactive_hook;
} hektor_t;

// Called by lua to restart the modem.
static int hektor_restart_modem_fn(lua_State *lua) {
  url_t restart_url;

  if (modem_get_restart_url(restart_url))
    modem_restart(restart_url);

  return 0;
}

static bool hektor_error_fetching_page(const url_t url) {
  printf("An error occured while fetching the page at ‘%s’.\n", url);

  return false;
}

static bool hektor_error_loading_config(hektor_t *hektor) {
  printf("An error occured while loading the configuration file: %s.\n",
         lua_get_error(&hektor->lua));

  return false;
}

static bool hektor_error_running_hook(hektor_t *hektor) {
  printf("An error occured while running configured hooks: %s\n",
         lua_get_error(&hektor->lua));

  return false;
}

static bool hektor_call_hook(hektor_t *hektor) {
  const info_t *info = &hektor->info;

  unit_conv_t allowed_unit;
  unit_conv_init(&allowed_unit, UNIT_BYTE, info_allowed_usage(info));

  if (info_modem_type(info) == MODEM_TYPE_9000)
    unit_conv_set_base(&allowed_unit, UNIT_BASE_BINARY);

  unit_conv_calculate(&allowed_unit);

  unit_conv_t remaining_unit;
  unit_conv_init(&remaining_unit, UNIT_BYTE, info_remaining_usage(info));

  if (info_modem_type(info) == MODEM_TYPE_9000)
    unit_conv_set_base(&remaining_unit, UNIT_BASE_BINARY);

  unit_conv_calculate(&remaining_unit);

  unit_conv_t refill_unit;
  unit_conv_set_amount(&refill_unit, info_refill_time(info));
  unit_conv_set_type(&refill_unit, UNIT_SECOND);
  unit_conv_calculate(&refill_unit);

  lua_table_elem_t elems[] = {
    {"allowed_mb", LUA_TNUMBER, {
      .number = unit_convert(info_allowed_usage(info), UNIT_BYTE,
                             UNIT_MEGABYTE)
    }},

    {"allowed_string", LUA_TSTRING, {
      .string = unit_conv_string(&allowed_unit)
    }},

    {"remaining_mb", LUA_TNUMBER, {
      .number = unit_convert(info_remaining_usage(info), UNIT_BYTE,
                             UNIT_MEGABYTE)
    }},

    {"remaining_string", LUA_TSTRING, {
      .string = unit_conv_string(&remaining_unit)
    }},

    {"remaining_pct", LUA_TNUMBER, {
      .number = 100 * ((double)info_remaining_usage(info) /
                               info_allowed_usage(info))
    }},

    {"refill_seconds", LUA_TNUMBER, {
      .number = info_refill_time(info)
    }},

    {"refill_timestamp", LUA_TNUMBER, {
      .number = info_refill_timestamp(info)
    }},

    {"refill_string", LUA_TSTRING, {
      .string = unit_conv_string(&refill_unit)
    }},

    {"restart_modem", LUA_TFUNCTION, {
      .function = hektor_restart_modem_fn
    }},

    {0}
  };

  if (info_fap_state(&hektor->info) == FAP_STATE_ACTIVE)
    return hook_call(&hektor->fap_is_active_hook, "t", &elems);
  else
    return hook_call(&hektor->fap_is_inactive_hook, "t", &elems);
}

static bool hektor_main(hektor_t *hektor) {
  url_t info_url;
  if (!modem_get_info_url(info_url))
    return false;

  page_t info_page;
  if (!modem_fetch_page(info_page, info_url))
    return hektor_error_fetching_page(info_url);

  if (!info_init(&hektor->info, info_page))
    return false;

  if (!config_init(&hektor->config, &hektor->lua))
    return false;

  hook_init(&hektor->fap_is_inactive_hook, &hektor->lua);
  hook_register(&hektor->fap_is_inactive_hook, "when_fap_is_inactive");

  hook_init(&hektor->fap_is_active_hook, &hektor->lua);
  hook_register(&hektor->fap_is_active_hook, "when_fap_is_active");

  if (!config_load(&hektor->config))
    return hektor_error_loading_config(hektor);

  if (!hektor_call_hook(hektor))
    return hektor_error_running_hook(hektor);

  return true;
}

static bool hektor_main_execute(int argc, char **argv) {
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
  return hektor_main_execute(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
