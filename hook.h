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

#ifndef HOOK_H
#define HOOK_H

#include <stdarg.h>
#include <stdbool.h>

#include "config.h"
#include "lua-util.h"

typedef struct {
  lua_t *lua;
  int lua_fn;
} hook_t;

// Initialize @hook with a @lua session.
void hook_init(hook_t *hook, lua_t *lua);

// Register @hook as a lua function called @fn_name.
void hook_register(hook_t *hook, const char *fn_name);

// Call the function assigned to @hook.
bool hook_call(hook_t *hook, const char *arg_spec, ...);

#endif
