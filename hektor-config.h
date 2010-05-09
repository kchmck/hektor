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

#ifndef HEKTOR_CONFIG_H
#define HEKTOR_CONFIG_H

#include <lua.h>
#include <stdbool.h>

#include "hektor-path.h"

enum { MAX_CONFIG_STRING_LENGTH = 32 };
typedef char config_string_t[MAX_CONFIG_STRING_LENGTH];

typedef struct {
  lua_State *lua;
  path_t storage_path;
} config_t;

// Load lua and config.
bool config_load(config_t *const config);

// Close config and lua.
static inline void config_close(config_t *const config) {
  lua_close(config->lua);
}

// Get a string option from the config, and copy it into buffer.
bool config_get_string(const char *const option_name, config_string_t buffer,
                       const config_t *const config);

#endif
