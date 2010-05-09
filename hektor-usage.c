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

#include "hektor-plan.h"
#include "hektor-snapshot.h"
#include "hektor-snapshots.h"
#include "hektor-span.h"
#include "hektor-usage.h"

double usage_calculate_remaining(const snapshots_t *const snapshots,
                                 const plan_t *const plan)
{
  // At least two snapshots are required for a calculation.
  if (snapshots->length < 2) return plan->threshold;

  double remaining = plan->threshold;

  int snapshot_pair = 0;
  const snapshot_t *begin, *end;

  // Loop through snapshot pairs.
  while (snapshots_get_pair(snapshots, &begin, &end, snapshot_pair++)) {
    span_t span;
    span_calculate_between(begin, end, plan, &span);

    remaining = span_calculate_remaining(remaining, plan, &span);
  }

  return remaining;
}
