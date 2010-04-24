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

#include "hektor-span.h"

// Guess if there was a reboot between two snapshots. When the modem reboots, it
// resets all usage numbers back to zero. If a later snapshot has lower usage
// numbers than an earlier one, you can figure the modem probably rebooted at
// some point between their recordings.
static inline bool possible_reboot_between(const snapshot_t *begin,
                                           const snapshot_t *end)
{
  return (end->upload < begin->upload) || (end->download < begin->download);
}

void span_calculate_between(const snapshot_t *begin, const snapshot_t *end,
                            const plan_t *plan, span_t *span)
{
  // Check if the snapshots are mix-matched (this happens inexplicably
  // sometimes.)
  if (end->snapshot_time < begin->snapshot_time)
    return span_calculate_between(end, begin, plan, span);

  span->elapsed = difftime(end->snapshot_time, begin->snapshot_time);

  if (possible_reboot_between(begin, end)) {
    span->uploaded = end->upload;
    span->downloaded = end->download;
  } else {
    span->uploaded = end->upload - begin->upload;
    span->downloaded = end->download - begin->download;
  }

  span->total_transferred = span->uploaded + span->downloaded;
  span->refilled = plan->refill_rate * span->elapsed;

  span->during_fap_free = snapshot_during_fap_free(begin)
                       && snapshot_during_fap_free(end);
}


double span_calculate_next(const double remaining, const plan_t *plan,
                           const span_t *span)
{
  // Only usage outside the fap-free period is subtracted.
  return min(span->during_fap_free ? remaining + span->refilled
                                   : remaining + span->refilled
                                               - span->total_transferred,
             plan->threshold);
}
