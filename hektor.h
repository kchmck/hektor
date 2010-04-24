#ifndef HEKTOR_H
#define HEKTOR_H

#include <stdio.h>
#include <stdbool.h>

#include "hektor-common.h"
#include "hektor-modem.h"
#include "hektor-path.h"
#include "hektor-plan.h"
#include "hektor-snapshots.h"
#include "hektor-snapshot.h"
#include "hektor-time.h"
#include "hektor-usage.h"

#include "config.h"

typedef struct {
  int argc;
  char **argv;

  snapshots_t snapshots;
  plan_t plan;
} hektor_t;

// Show a help message.
bool hektor_cmd_help(hektor_t *hektor);

// Show the remaining usage in megabytes.
bool hektor_cmd_remaining(hektor_t *hektor);

// Record a new snapshot and show the remaining usage.
bool hektor_cmd_record(hektor_t *hektor);

// Remove a certain amount (default 1) of snapshots off the snapshots list.
bool hektor_cmd_pop(hektor_t *hektor);

// Show some statistics about the recorded snapshots.
bool hektor_cmd_stats(hektor_t *hektor);

// Show span information.
bool hektor_cmd_list(hektor_t *hektor);

// A command function
typedef bool (*hektor_cmd_fn_t)(hektor_t *);

// The list of commands
enum { HEKTOR_CMDS_LENGTH = 8 };

static const struct {
  const char *command_name;
  const hektor_cmd_fn_t command_fn;
} hektor_cmds[HEKTOR_CMDS_LENGTH] = {
  {"remaining", hektor_cmd_remaining},
  {"record",    hektor_cmd_record},
  {"pop",       hektor_cmd_pop},
  {"stats",     hektor_cmd_stats},
  {"list",      hektor_cmd_list},

  {"-h",        hektor_cmd_help},
  {"--help",    hektor_cmd_help},
  {"help",      hektor_cmd_help}
};

// Find the right command and run it.
bool hektor_cmd_handle(hektor_t *hektor);

// Main...
int main(int argc, char **argv);

#endif
