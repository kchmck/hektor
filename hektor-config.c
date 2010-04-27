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

#include <lauxlib.h>
#include <lualib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "hektor-config.h"
#include "hektor-path.h"

static bool write_default_config(const config_t *config) {
  static const char DEFAULT_CONFIG[] =
    "-- Your Hughesnet usage plan, which can be one of: home, pro, pro-plus,\n"
    "-- elite, elite-plus, or elite-premium.\n"
    "usage_plan = \"home\"\n";

  // Strip off the trailing \0.
  enum { DEFAULT_CONFIG_ITEMS = sizeof(DEFAULT_CONFIG) - 1 };

  FILE *config_file = fopen(config->storage_path, "w");
  if (!config_file) return false;

  const size_t written = fwrite(DEFAULT_CONFIG, sizeof(char),
                                DEFAULT_CONFIG_ITEMS, config_file);
  fclose(config_file);

  return written == DEFAULT_CONFIG_ITEMS;
}

bool config_load(config_t *config) {
  config->lua = lua_open();
  if (!config->lua) return false;
  luaL_openlibs(config->lua);

  if (!path_make_config_storage(config->storage_path)) return false;

  switch (luaL_loadfile(config->lua, config->storage_path)) {
    case 0:
      break;
    case LUA_ERRFILE:
      // Write a default config and reload the file.
      return write_default_config(config) && config_load(config);
    default:
      return false;
  }

  return lua_pcall(config->lua, 0, 0, 0) == 0;
}

bool config_get_string(const char *option_name, config_string_t buffer,
                       const config_t *config)
{
  lua_getglobal(config->lua, option_name);

  if (!lua_isstring(config->lua, -1) || lua_isnumber(config->lua, -1))
    return false;

  strncpy(buffer, lua_tostring(config->lua, -1), MAX_CONFIG_STRING_LENGTH);

  return true;
}
