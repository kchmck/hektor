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

#ifndef HEKTOR_SPAN_H
#define HEKTOR_SPAN_H

#include <time.h>

#include "hektor-common.h"
#include "hektor-plan.h"
#include "hektor-snapshot.h"

typedef struct {
  time_t elapsed;
  bool during_fap_free;

  long uploaded;
  long downloaded;

  long total_usage;
  long counted_usage;

  double refilled;
} span_t;

// Calculate the difference between two snapshots.
void span_calculate_between(const snapshot_t *const begin,
                            const snapshot_t *const end,
                            const plan_t *const plan, 
                            span_t *const span);

// Apply a snapshot's usage information to a remaining value and return the
// result.
static inline double span_calculate_remaining(const double currently_remaining,
                                              const plan_t *const plan,
                                              const span_t *const span)
{
  // The amount refilled never exceeds the plan's bandwidth threshold.
  return min(currently_remaining + span->refilled - span->counted_usage,
             plan->threshold);
}

#endif
