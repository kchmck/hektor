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
#include <stdio.h>
#include <string.h>

#include <lauxlib.h>
#include <lualib.h>

#include "hektor-common.h"
#include "hektor-config.h"
#include "hektor-path.h"

static bool write_default_config(const config_t *config) {
  static const char DEFAULT_CONFIG[] =
    "-- Your Hughesnet usage plan, which can be one of: home, pro, pro-plus,\n"
    "-- elite, elite-plus, or elite-premium.\n"
    "usage_plan = \"home\"\n";

  // DEFAULT_CONFIG's length, not counting the ending '\0'
  enum { DEFAULT_CONFIG_LENGTH = sizeof(DEFAULT_CONFIG) - 1 };

  FILE *config_file = fopen(config->storage_path, "w");
  if (!config_file) return false;

  const size_t written = fwrite(DEFAULT_CONFIG, sizeof(char),
                                DEFAULT_CONFIG_LENGTH, config_file);
  fclose(config_file);

  return written == DEFAULT_CONFIG_LENGTH;
}

bool config_load(config_t *config) {
  // Init lua...
  config->lua = lua_open();
  if (!config->lua) return false;
  luaL_openlibs(config->lua);

  // Get the path to config.
  if (!path_make_config_storage(config->storage_path)) return false;

  // Load the file into lua.
  switch (luaL_loadfile(config->lua, config->storage_path)) {
    // No error
    case 0:           break;
    // The file doesn't exist; write a default config and reload the file.
    case LUA_ERRFILE: return write_default_config(config)
                          && config_load(config);
    // Any other error
    default:          return false;
  }

  // Interpret its contents.
  return lua_pcall(config->lua, 0, 0, 0) == 0;
}

bool config_get_string(const char *option_name, config_string_t buffer,
                       const config_t *config)
{
  lua_getglobal(config->lua, option_name);

  // In lua, a number is also considered a string, but we don't want that here.
  if (!lua_isstring(config->lua, -1) || lua_isnumber(config->lua, -1))
    return false;

  string_copy(lua_tostring(config->lua, -1), buffer, MAX_CONFIG_STRING_LENGTH);

  return true;
}
