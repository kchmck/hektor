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

#include "hektor.h"

bool hektor_cmd_help(hektor_t *hektor) {
  printf("Usage: %s [COMMAND], where COMMAND is one of:\n"
         "  remaining  Show the amount of megabytes remaining before the FAP\n"
         "             is activated (this is the default command.)\n"
         "     record  Record a new usage snapshot.\n"
         "        pop  Remove the last recorded snapshot.\n"
         "      stats  Display snapshot statistics.\n"
         "       list  Display detailed usage information.\n",

         hektor->argv[0]);

  return true;
}

bool hektor_cmd_remaining(hektor_t *hektor) {
  printf("%.2f megabytes remaining\n",
    usage_calculate_remaining(&hektor->snapshots,
                              &hektor->plan) / 1000 / 1000);

  return true;
}

bool hektor_cmd_record(hektor_t *hektor) {
  // Download the whole menu page.
  char menu_page[MAX_PAGE_SIZE] = {0};
  if (!modem_fetch_menu_page(menu_page)) return false;

  // Find the url to the pep page.
  char pep_url[MAX_PATH_LENGTH] = {0};
  if (!modem_find_pep_url(menu_page, pep_url)) return false;

  // Download the pep page.
  char pep_page[MAX_PAGE_SIZE] = {0};
  if (!modem_fetch_page(pep_url, pep_page)) return false;

  // Get the next empty snapshot.
  snapshot_t *snapshot = snapshots_next_empty(&hektor->snapshots);
  if (!snapshot) return false;

  // Record a new snapshot.
  snapshot_record(snapshot, pep_page);

  printf("Recorded a snapshot\n");

  // Show the remaining usage.
  return hektor_cmd_remaining(hektor);
}

bool hektor_cmd_drop(hektor_t *hektor) {
  // See if the user gave an amount to remove or default to 1.
  const int snapshots_to_remove = hektor->argc > 2 ? atoi(hektor->argv[2]) : 1;

  // Check for sanity.
  if (hektor->snapshots.length < snapshots_to_remove
  ||  snapshots_to_remove <= 0)
    return false;

  // This is pretty easy.
  hektor->snapshots.length -= snapshots_to_remove;

  printf("Removed %d %s\n", snapshots_to_remove,
    snapshots_to_remove == 1 ? "snapshot" : "snapshots");

  return true;
}

bool hektor_cmd_stats(hektor_t *hektor) {
  const snapshots_t *snapshots = &hektor->snapshots;

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

  const time_t first = snapshots->list[0].snapshot_time;
  const time_t last = snapshots->list[snapshots->length - 1].snapshot_time;

  char first_formatted[MAX_TIME_FORMAT_LENGTH] = {0};
  char last_formatted[MAX_TIME_FORMAT_LENGTH] = {0};

  if (!format_time(&first, first_formatted)
  ||  !format_time(&last, last_formatted))
    return false;

  printf("The earliest was recorded at:\n"
         "  %s\n"
         "and the latest at:\n"
         "  %s\n",
         first_formatted, last_formatted);

  const double diff_hours = difftime(last, first) / 60 / 60;

  printf("That equals %.2f hours of usage history.\n", diff_hours);

  return true;
}

bool hektor_cmd_list(hektor_t *hektor) {
  const snapshots_t *snapshots = &hektor->snapshots;
  const plan_t *plan = &hektor->plan;

  if (snapshots->length < 2) {
    printf("At least two snapshots are required for a listing.\n");
    return true;
  }

  double remaining = plan->threshold;
  int pair_index = 0;

  while (true) {
    const snapshot_t *begin, *end;
    if (!snapshots_get_pair(snapshots, &begin, &end, pair_index++)) break;

    char from_time[MAX_TIME_FORMAT_LENGTH] = {0};
    char to_time[MAX_TIME_FORMAT_LENGTH] = {0};

    if (!format_time(&begin->snapshot_time, from_time)
    ||  !format_time(&end->snapshot_time, to_time))
      continue;

    span_t span = {0};
    span_calculate_between(begin, end, plan, &span);

    printf("from %s\n"
           "  to %s\n"
           "\n"
           "   %6.2f megabytes remaining before\n"
           " - %6.2f megabytes transferred\n"
           " + %6.2f megabytes refilled\n",

           from_time, to_time,
           remaining / 1000 / 1000,
           (double)span.total_transferred / 1000 / 1000,
           span.refilled / 1000 / 1000);

    remaining = span_calculate_next(remaining, plan, &span);

    printf(" = %6.2f megabytes remaining after\n", remaining / 1000 / 1000);

    // Don't print a separator after the last span.
    if (pair_index != snapshots->length - 1) printf("---\n");
  }

  return true;
}

bool hektor_cmd_handle(hektor_t *hektor) {
  // Run the default command if no other command was given.
  if (hektor->argc < 2) return hektor_cmd_remaining(hektor);

  const char *command_name = hektor->argv[1];

  for (int i = 0; i < HEKTOR_CMDS_LENGTH; ++i)
    if (strcmp(command_name, hektor_cmds[i].command_name) == 0)
      return hektor_cmds[i].command_fn(hektor);

  hektor_cmd_help(hektor);
  printf("'%s' is an invalid command.\n", command_name);

  return false;
}

int main(int argc, char **argv) {
  hektor_t hektor = {argc, argv};

  if (!plan_load(&hektor.plan, HUGHESNET_PLAN)
  ||  !snapshots_load(&hektor.snapshots))
    return 1;

  const bool success = hektor_cmd_handle(&hektor);
  if (!snapshots_save(&hektor.snapshots)) return 1;

  return success ? 0 : 1;
}
