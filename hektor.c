// Copyright 2010 Mick Koch <kchmck@gmail.com>
//
// This file is part of Hektor.
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
// Hektor. If not, see <http://www.gnu.org/licenses/>.

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hektor-config.h"
#include "hektor-error.h"
#include "hektor-modem.h"
#include "hektor-path.h"
#include "hektor-plan.h"
#include "hektor-snapshot.h"
#include "hektor-snapshots.h"
#include "hektor-span.h"
#include "hektor-time.h"
#include "hektor-usage.h"

typedef struct {
  int argc;
  char **argv;

  snapshots_t snapshots;
  plan_t plan;
} hektor_t;

static bool hektor_cmd_help(hektor_t *hektor) {
  printf("Usage: %s [COMMAND=remaining] [ARGS], where COMMAND can be:\n"
         "  remaining        Show the amount of megabytes remaining\n"
         "                   before the FAP is activated.\n"
         "  record           Record a new usage snapshot.\n"
         "  drop [AMOUNT=1]  Remove a certain AMOUNT of newly-recorded\n"
         "                   snapshots.\n"
         "  stats            Display snapshot statistics.\n"
         "  list             Display detailed usage information.\n",

         hektor->argv[0]);

  return true;
}

static bool hektor_cmd_remaining(hektor_t *hektor) {
  printf("%.2f megabytes remaining\n",
    usage_calculate_remaining(&hektor->snapshots,
                              &hektor->plan) / 1000 / 1000);

  return true;
}

static bool hektor_cmd_record(hektor_t *hektor) {
  // Download the whole menu page.
  page_t menu_page;
  if (!modem_fetch_menu_page(menu_page)) return false;

  // Find the url to the pep page.
  url_t pep_url;
  if (!modem_find_pep_url(menu_page, pep_url)) return false;

  // Download the pep page.
  page_t pep_page;
  if (!modem_fetch_page(pep_url, pep_page)) return false;

  // Get the next empty snapshot.
  snapshot_t *snapshot = snapshots_next_empty(&hektor->snapshots);
  if (!snapshot) return false;

  // Record a new snapshot.
  snapshot_record(snapshot, pep_page);

  printf("Recorded snapshot number %d.\n", hektor->snapshots.length);

  // Show the remaining usage.
  return hektor_cmd_remaining(hektor);
}

static bool hektor_cmd_drop(hektor_t *hektor) {
  snapshots_t *snapshots = &hektor->snapshots;

  // See if the user gave an amount to remove or default to 1.
  const int snapshots_to_remove = hektor->argc > 2 ? atoi(hektor->argv[2]) : 1;

  // Check for sanity.
  if (snapshots->length < snapshots_to_remove || snapshots_to_remove <= 0)
    return false;

  for (int i = 0; i < snapshots_to_remove; i += 1) {
    const snapshot_t *snapshot = snapshots_get_last(snapshots);

    time_format_t snapshot_time;
    if (!format_time(&snapshot->snapshot_time, snapshot_time)) continue;

    printf("Removed snapshot number %d, recorded on %s.\n", snapshots->length,
                                                            snapshot_time);
    snapshots->length -= 1;
  }

  return true;
}

static bool hektor_cmd_stats(hektor_t *hektor) {
  snapshots_t *snapshots = &hektor->snapshots;

  printf("%d %s %s been recorded. ", snapshots->length,
                                     snapshots->length == 1 ? "snapshot"
                                                            : "snapshots",
                                     snapshots->length == 1 ? "has"
                                                            : "have");

  // 2 or more snapshots are required for more details.
  if (snapshots->length < 2) {
    printf("Record at least two snapshots for some\n"
           "more details.\n");

    return true;
  }

  const snapshot_t *first = snapshots_get_first(snapshots);
  const snapshot_t *last = snapshots_get_last(snapshots);

  span_t span;
  span_calculate_between(first, last, &hektor->plan, &span);

  time_format_t first_time;
  time_format_t last_time;

  if (!format_time(&first->snapshot_time, first_time)
  ||  !format_time(&last->snapshot_time, last_time))
    return false;

  printf("The earliest was recorded on:\n"
         "  %s\n"
         "and the latest on:\n"
         "  %s\n"
         "That equals %.2f hours of usage history.\n",

         first_time, last_time,
         (double)span.elapsed / 60 / 60);

  return true;
}

static bool hektor_cmd_list(hektor_t *hektor) {
  const snapshots_t *snapshots = &hektor->snapshots;
  const plan_t *plan = &hektor->plan;

  if (snapshots->length < 2) {
    printf("At least two snapshots are required for a listing.\n");
    return true;
  }

  double remaining = plan->threshold;

  int snapshot_pair = 0;
  const snapshot_t *begin, *end;

  while (snapshots_get_pair(snapshots, &begin, &end, snapshot_pair++)) {
    time_format_t from_time;
    time_format_t to_time;

    if (!format_time(&begin->snapshot_time, from_time)
    ||  !format_time(&end->snapshot_time, to_time))
      continue;

    span_t span;
    span_calculate_between(begin, end, plan, &span);

    printf("from %s\n"
           "  to %s\n"
           "\n"
           "   %6.2f megabytes remaining before\n"
           " - %6.2f megabytes transferred\n"
           " + %6.2f megabytes refilled\n",

           from_time, to_time,
           remaining / 1000 / 1000,
           (double)span.counted_usage / 1000 / 1000,
           span.refilled / 1000 / 1000);

    remaining = span_calculate_remaining(remaining, plan, &span);

    printf(" = %6.2f megabytes remaining after\n", remaining / 1000 / 1000);

    // Don't print a separator after the last span.
    if (snapshot_pair + 1 != snapshots->length) printf("---\n");
  }

  return true;
}

typedef bool (*hektor_cmd_fn_t)(hektor_t *);

// The list of commands
enum { HEKTOR_CMDS_LENGTH = 8 };

static const struct {
  const char *command_name;
  const hektor_cmd_fn_t command_fn;
} hektor_cmds[HEKTOR_CMDS_LENGTH] = {
  {"remaining", hektor_cmd_remaining},
  {"record",    hektor_cmd_record},
  {"drop",      hektor_cmd_drop},
  {"stats",     hektor_cmd_stats},
  {"list",      hektor_cmd_list},

  {"-h",        hektor_cmd_help},
  {"--help",    hektor_cmd_help},
  {"help",      hektor_cmd_help}
};

static bool hektor_cmd_handle(hektor_t *hektor) {
  // Run the default command if no other command was given.
  if (hektor->argc < 2) return hektor_cmd_remaining(hektor);

  const char *command_name = hektor->argv[1];

  for (int i = 0; i < HEKTOR_CMDS_LENGTH; i += 1) {
    const char *current_command = hektor_cmds[i].command_name;

    // Try to match a partial command name, so 'rem' will match 'remaining',
    // etc.
    if (strstr(current_command, command_name) == &current_command[0])
      return hektor_cmds[i].command_fn(hektor);
  }

  hektor_cmd_help(hektor);

  return hektor_error_invalid_command(command_name);
}

static bool hektor_main(int argc, char **argv) {
  hektor_t hektor = {argc, argv};

  config_t config;
  if (!config_load(&config))
    return hektor_error_loading_config(&config);

  config_string_t plan_name;
  if (!config_get_string("usage_plan", plan_name, &config))
    return hektor_error_loading_plan(&config);

  config_close(&config);

  if (!plan_load(plan_name, &hektor.plan))
    return hektor_error_invalid_plan(plan_name);

  if (!snapshots_load(&hektor.snapshots))
    return hektor_error_loading_snapshots(&hektor.snapshots);

  const bool cmd_result = hektor_cmd_handle(&hektor);

  if (!snapshots_save(&hektor.snapshots))
    return hektor_error_saving_snapshots(&hektor.snapshots);

  return cmd_result;
}

int main(int argc, char **argv) {
  return hektor_main(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
