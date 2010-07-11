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
#include <stddef.h>
#include <stdio.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "config.h"
#include "lua-util.h"
#include "path.h"

bool config_init(config_t *config, lua_t *lua) {
  config->lua = lua;

  return get_config_paths(config->config_dir, config->config_file) &&
         make_dir_and_parents(config->config_dir);
}

// Write a default config file.
static bool config_create_default(config_t *config) {
  static const char DEFAULT_CONFIG[] =
  "-- This hook is called when the FAP is inactive. When usage gets uncomfortably\n"
  "-- low, steps can be taken to prevent FAP activation. One step could be\n"
  "-- repeatedly restarting the modem.\n"
  "when_fap_is_inactive(function (hektor)\n"
  "\n"
  "  print(hektor.remaining_string .. \" are remaining\")\n"
  "\n"
  "  -- if hektor.remaining_usage < 5 then\n"
  "  --   hektor.restart_modem()\n"
  "  -- end\n"
  "\n"
  "end)\n"
  "\n"
  "-- This hook is called when the FAP is active.\n"
  "when_fap_is_active(function (hektor)\n"
  "\n"
  "  print(hektor.refill_string .. \" until FAP deactivation (at \" ..\n"
  "        os.date(\"%I:%M %p %A\", hektor.refill_timestamp) ..  \")\")\n"
  "\n"
  "end)\n";

  enum { DEFAULT_CONFIG_LENGTH = sizeof(DEFAULT_CONFIG) - 1 };

  FILE *config_file = fopen(config->config_file, "w");
  if (!config_file) return false;

  const size_t written = fwrite(DEFAULT_CONFIG, sizeof(char),
                                DEFAULT_CONFIG_LENGTH, config_file);
  fclose(config_file);

  return written == DEFAULT_CONFIG_LENGTH;
}

// Load a default config file.
static inline bool config_load_default(config_t *config) {
  return config_create_default(config) && config_load(config);
}

bool config_load(config_t *config) {
  switch (luaL_loadfile(lua_state(config->lua), config->config_file)) {
    // No error
    case 0:           break;

    // The file doesn't exist; write a default config and reload the file.
    case LUA_ERRFILE: return config_load_default(config);

    // Any other error
    default:          return false;
  }

  // Interpret its contents.
  return lua_pcall(lua_state(config->lua), 0, 0, 0) == 0;
}
