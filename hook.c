#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>

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

bool hook_call(hook_t *hook, const char *arg_format, ...) {
  lua_State *lua = lua_state(hook->lua);
  lua_rawgeti(lua, LUA_REGISTRYINDEX, hook->lua_fn);

  va_list args;
  va_start(args, arg_format);

  const int num_args = lua_push_args(lua, arg_format, args);

  va_end(args);

  return lua_pcall(lua, num_args, 0, 0) == 0;
}
