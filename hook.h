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

// Initialize a @hook with a @lua session.
void hook_init(hook_t *hook, lua_t *lua);

// Register a @hook as a lua function called @fn_name.
void hook_register(hook_t *hook, const char *fn_name);

// Call the function assigned to @hook.
bool hook_call(hook_t *hook, const char *arg_format, ...);

#endif
