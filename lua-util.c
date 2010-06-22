#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

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

void lua_closure_register(lua_State *lua, const char *fn_name,
                          lua_CFunction fn, const int num_userdata, ...)
{
  va_list userdata;
  va_start(userdata, num_userdata);

  for (int i = 0; i < num_userdata; i += 1)
    lua_pushlightuserdata(lua, va_arg(userdata, void *));

  va_end(userdata);

  lua_pushcclosure(lua, fn, num_userdata);
  lua_setglobal(lua, fn_name);
}

int lua_push_args(lua_State *lua, const char *arg_format, va_list args) {
  int arg_index = 0;
  char arg = 0;

  while ((arg = arg_format[arg_index])) {
    switch (arg) {
    case 'b': lua_pushboolean(lua, va_arg(args, bool));  break;
    case 'i': lua_pushinteger(lua, va_arg(args, int));   break;
    case 'l': lua_pushinteger(lua, va_arg(args, long));  break;
    case 'f': lua_pushnumber(lua, va_arg(args, double)); break;
    case 's': lua_pushstring(lua, va_arg(args, char *)); break;
    }

    arg_index += 1;
  }

  return arg_index;
}
