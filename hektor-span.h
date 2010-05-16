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
  // The seconds elapsed between snapshots
  time_t elapsed;
  // True if both snapshots were recorded during fap-free
  bool during_fap_free;

  // Total bytes uploaded
  long uploaded;
  // Total bytes downloaded
  long downloaded;

  // Combined bytes uploaded and downloaded
  long total_usage;
  // The amount of bytes actually counted against the fap (hughesnet doesn't
  // count uploads nor any usage inside the fap-free period)
  long counted_usage;

  // Total bytes refilled
  double refilled;
} span_t;

// Calculate the difference between two snapshots.
void span_calculate_between(const snapshot_t *begin, const snapshot_t *end,
                            const plan_t *plan, span_t *span);

// Apply a snapshot's usage information to a remaining value and return the
// result.
double span_calculate_remaining(const double currently_remaining,
                                const plan_t *plan,
                                const span_t *span);

#endif
