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

#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

#include "lua-util.h"
#include "path.h"

typedef struct {
  lua_t *lua;

  path_t config_dir;
  path_t config_file;
} config_t;

// Initialize a new @config structure with a @lua session.
bool config_init(config_t *config, lua_t *lua);

// Load config from the disk.
bool config_load(config_t *config);

#endif
