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
#include <string.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "config.h"
#include "lua-util.h"
#include "common.h"
#include "path.h"

bool config_init(config_t *config, lua_t *lua, const char *def_config) {
  config->lua = lua;
  config->def_config = def_config;

  return path_build_config(config->config_dir, config->config_file) &&
         path_make_dirs(config->config_dir);
}

// Write a default config file.
static bool config_create_default(config_t *config)
{
  FILE *config_file = fopen(config->config_file, "w");
  if (!config_file) return false;

  const size_t written = fputs(config->def_config, config_file);
  fclose(config_file);

  return written > 0;
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
