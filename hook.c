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

#include <stdarg.h>
#include <stdbool.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "common.h"
#include "hook.h"
#include "lua-util.h"

void hook_init(hook_t *hook, lua_t *lua) {
  hook->lua = lua;
  hook->lua_fn = 0;
}

// Called by lua to assign a function to a hook.
static int hook_fn(lua_State *lua) {
  hook_t *hook = (hook_t *)lua_closure_get_userdata(lua, 1);

  if (!hook || !lua_isfunction(lua, 1))
    return 0;

  hook->lua_fn = luaL_ref(lua, LUA_REGISTRYINDEX);

  return 0;
}

void hook_register(hook_t *hook, const char *fn_name) {
  lua_closure_register(lua_state(hook->lua), fn_name, hook_fn, 1, hook);
}

bool hook_call(hook_t *hook, const char *arg_spec, ...) {
  if (!hook->lua_fn)
    return false;

  lua_State *lua_s = lua_state(hook->lua);
  lua_rawgeti(lua_s, LUA_REGISTRYINDEX, hook->lua_fn);

  va_list args;
  va_start(args, arg_spec);

  const int num_args = lua_push_args(lua_s, arg_spec, args);

  va_end(args);

  return lua_pcall(lua_s, num_args, 0, 0) == 0;
}
