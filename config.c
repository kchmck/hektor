#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "config.h"
#include "lua-util.h"
#include "path.h"

bool config_init(config_t *config, lua_t *lua) {
  config->lua = lua;

  return get_config_paths(config->config_dir, config->config_file) &&
         make_dir(config->config_dir);
}

// Write a default config file.
static bool config_create_default(config_t *config) {
  static const char DEFAULT_CONFIG[] =
  "-- The remaining hook is called when the FAP is not active. The hooked\n"
  "-- function recieves the megabytes remaining and a formatted string\n"
  "-- like '10.3 megabytes'.\n"
  "hook_remaining(function (mb_remaining, remaining_string)\n"
  "  print(string.format('%s are remaining.', remaining_string))\n"
  "\n"
  "  if mb_remaining < 5 then\n"
  "    -- Do something useful when the remaining usage goes below 5 megabytes.\n"
  "  end\n"
  "end)\n"
  "\n"
  "-- The refill hook is called when the FAP is active. The hooked function\n"
  "-- recieves the seconds until refill, the time of refill as a unix\n"
  "-- timestamp, and a formatted string like '30.0 minutes'.\n"
  "hook_refill(function (refill_secs, refill_timestamp, refill_string)\n"
  "  print(string.format('The FAP will be deactivated in %s, at %s.',\n"
  "                      refill_string, os.date('%I:%M %p %A', timestamp)))\n"
  "end)\n";

  enum { DEFAULT_CONFIG_LENGTH = sizeof(DEFAULT_CONFIG) - 1 };

  FILE *config_file = fopen(config->config_file, "w");
  if (!config_file) return false;

  const size_t written = fwrite(DEFAULT_CONFIG, sizeof(char),
                                DEFAULT_CONFIG_LENGTH, config_file);
  fclose(config_file);

  return written == DEFAULT_CONFIG_LENGTH;
}

// Load a default config file.
static inline bool config_load_default(config_t *config) {
  return config_create_default(config) && config_load(config);
}

bool config_load(config_t *config) {
  switch (luaL_loadfile(lua_state(config->lua), config->config_file)) {
  // No error
  case 0:           break;

  // The file doesn't exist; write a default config and reload the file.
  case LUA_ERRFILE: return config_load_default(config);

  // Any other error
  default:          return false;
  }

  // Interpret its contents.
  return lua_pcall(lua_state(config->lua), 0, 0, 0) == 0;
}
