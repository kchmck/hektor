#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "lua-util.h"
#include "path.h"

typedef struct {
  lua_t *lua;

  path_t config_dir;
  path_t config_file;
} config_t;

static inline const char *config_file_path(const config_t *config) {
  return config->config_file;
}

// Initialize a new @config structure with a @lua session.
bool config_init(config_t *config, lua_t *lua);

// Load config from the disk.
bool config_load(config_t *config);

#endif
