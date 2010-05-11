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
#include <time.h>

#include "hektor-plan.h"
#include "hektor-snapshot.h"

#include "hektor-span.h"

// Guess if there was a reboot between two snapshots. When the modem reboots, it
// resets all usage numbers back to zero. If a later snapshot has lower usage
// numbers than an earlier one, you can figure the modem probably rebooted at
// some point between their recordings.
static inline bool possible_reboot_between(const snapshot_t *const begin,
                                           const snapshot_t *const end)
{
  return (end->upload < begin->upload) || (end->download < begin->download);
}

void span_calculate_between(const snapshot_t *const begin,
                            const snapshot_t *const end,
                            const plan_t *const plan,
                            span_t *const span)
{
  // Check if the snapshots are mix-matched (this happens inexplicably
  // sometimes.)
  if (end->snapshot_time < begin->snapshot_time)
    return span_calculate_between(end, begin, plan, span);

  span->elapsed = difftime(end->snapshot_time, begin->snapshot_time);
  span->during_fap_free = snapshot_during_fap_free(begin)
                       && snapshot_during_fap_free(end);

  if (possible_reboot_between(begin, end)) {
    span->uploaded = end->upload;
    span->downloaded = end->download;
  } else {
    span->uploaded = end->upload - begin->upload;
    span->downloaded = end->download - begin->download;
  }

  span->total_usage = span->downloaded + span->uploaded;
  span->counted_usage = span->during_fap_free ? 0 : span->downloaded;

  span->refilled = plan->refill_rate * span->elapsed;
}

double span_calculate_remaining(const double currently_remaining,
                                const plan_t *const plan,
                                const span_t *const span)
{
  // The amount refilled never exceeds the plan's bandwidth threshold.
  return min(currently_remaining + span->refilled - span->counted_usage,
             plan->threshold);
}
