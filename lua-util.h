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

#ifndef LUA_UTIL_H
#define LUA_UTIL_H

#include <stdarg.h>
#include <stdbool.h>

#include <lua.h>
#include <lualib.h>

typedef struct {
  lua_State *state;
} lua_t;

// Get the raw lua state of @lua.
static inline lua_State *lua_state(lua_t *lua) {
  return lua->state;
}

// Initialize @lua.
bool lua_init(lua_t *lua);

// Destroy @lua.
void lua_destroy(lua_t *lua);

// Register @fn as a closure called @fn_name with userdata that can be retrieved
// with @lua_closure_get_userdata.
void lua_closure_register(lua_State *lua, const char *fn_name, lua_CFunction fn,
                          const int num_userdata, ...);

// Get the userdata assigned to a closure at @userdata_idx.
static inline void *lua_closure_get_userdata(lua_State *lua,
                                             const int userdata_idx)
{
  return lua_touserdata(lua, lua_upvalueindex(userdata_idx));
}

// Get the most recent error from @lua.
static inline const char *lua_last_error(lua_t *lua) {
  return lua_tostring(lua_state(lua), -1);
}

typedef struct {
  const char *key;
  const int value_type;

  const union {
    const lua_Number number;
    const char *string;
    const lua_CFunction function;
  } value;
} lua_table_elem_t;

typedef lua_table_elem_t lua_table_t[];

enum { LUA_TABLE_END = 0 };

// Build a new lua table on top of the stack and fill it with the elements in
// @table.
//
// The following example would build a table with two keys: "a" -- a number with
// the value 42, and "b" -- a string with the value "wut".
//
//   const lua_table_t table = {
//     {"a", LUA_TNUMBER, {
//       .number = 42
//     }},
//     {"b", LUA_TSTRING, {
//       .string = "wut"
//     }}
//   };
//
//   lua_build_table(a_lua_State, table);
//
void lua_build_table(lua_State *lua, const lua_table_t table);

// Push a variable number of @args based on @arg_spec. @arg_spec is a string of
// characters that describes the type of each argument:
//
//   b  int or bool
//   i  int
//   l  long
//   f  double
//   s  char *
//   t  lua_table_t
//
//   @return: the number of arguments pushed.
//
//   lua_push_args(a_lua_State, "bits", true, 42, table, "hallo") -> 4
//
int lua_push_args(lua_State *lua, const char *arg_spec, va_list args);

#endif
