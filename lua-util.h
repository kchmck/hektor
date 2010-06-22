#ifndef LUA_UTIL_H
#define LUA_UTIL_H

#include <stdarg.h>
#include <stdbool.h>

#include <lua.h>

typedef struct {
  lua_State *state;
} lua_t;

// Get the raw lua state from a @lua session.
static inline lua_State *lua_state(lua_t *lua) {
  return lua->state;
}

// Initialize a new @lua session.
bool lua_init(lua_t *lua);

// Destroy a @lua session.
void lua_destroy(lua_t *lua);

// Register @fn as a closure called @fn_name with userdata that can be retrieved
// with @lua_closure_get_userdata.
void lua_closure_register(lua_State *lua, const char *fn_name,
                          lua_CFunction fn, const int num_userdata, ...);

// Get the userdata assigned to a closure at @userdata_idx.
void *lua_closure_get_userdata(lua_State *lua, const int userdata_idx);

static inline const char *lua_get_error(lua_State *lua) {
  return lua_tostring(lua, -1);
}

// Push a variable number of @args onto the lua stack. @arg_format is a string
// that describes each argument's type. Each of its characters can be one of:
//
//   'b' for boolean
//   'i' for integer
//   'l' for long integer
//   'f' for float or double
//   's' for string
//
int lua_push_args(lua_State *lua, const char *arg_format, va_list args);

#endif
