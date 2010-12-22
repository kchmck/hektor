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

#include "lua-util.h"

bool lua_init(lua_t *lua) {
  lua->state = lua_open();
  if (!lua->state) return false;

  luaL_openlibs(lua->state);

  return true;
}

void lua_destroy(lua_t *lua) {
  lua_close(lua->state);
}

void lua_closure_register(lua_State *lua, const char *fn_name, lua_CFunction fn,
                          const int num_userdata, ...)
{
  va_list userdata;
  va_start(userdata, num_userdata);

  for (int i = 0; i < num_userdata; i += 1)
    lua_pushlightuserdata(lua, va_arg(userdata, void *));

  va_end(userdata);

  lua_pushcclosure(lua, fn, num_userdata);
  lua_setglobal(lua, fn_name);
}

void lua_build_table(lua_State *lua, const lua_table_elem_t elems[]) {
  // Push a new table onto the stack and store its index.
  lua_newtable(lua);
  const int table_index = lua_gettop(lua);

  int elem_index = 0;
  const lua_table_elem_t *elem;

  while ((elem = &elems[elem_index])->key != LUA_TABLE_END) {
    switch (elem->value_type) {
      case LUA_TNUMBER:
        lua_pushnumber(lua, elem->value.number);
        break;

      case LUA_TSTRING:
        lua_pushstring(lua, elem->value.string);
        break;

      case LUA_TFUNCTION:
        lua_pushcfunction(lua, elem->value.function);
    }

    lua_setfield(lua, table_index, elem->key);
    elem_index += 1;
  }
}

int lua_push_args(lua_State *lua, const char *arg_spec, va_list args) {
  int arg_index = 0;
  char arg;

  while ((arg = arg_spec[arg_index])) {
    switch (arg) {
      case 'b':
        lua_pushboolean(lua, va_arg(args, int));
        break;

      case 'i':
        lua_pushinteger(lua, va_arg(args, int));
        break;

      case 'l':
        lua_pushinteger(lua, va_arg(args, long));
        break;

      case 'f':
        lua_pushnumber(lua, va_arg(args, double));
        break;

      case 's':
        lua_pushstring(lua, va_arg(args, char *));
        break;

      case 't':
        lua_build_table(lua, va_arg(args, lua_table_elem_t *));
    }

    arg_index += 1;
  }

  return arg_index;
}
