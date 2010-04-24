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

#include "hektor-usage.h"

double usage_calculate(const snapshots_t *snapshots, const plan_t *plan) {
  // At least two snapshots are required for a calculation.
  if (snapshots->length < 2) return plan->threshold;

  double remaining = plan->threshold;
  int pair_index = 0;

  // Loop through snapshot pairs.
  while (true) {
    const snapshot_t *begin, *end;
    if (!snapshots_get_pair(snapshots, &begin, &end, pair_index++)) break;

    span_t span = {0};
    span_calculate_between(begin, end, plan, &span);

    remaining = span_calculate_next(remaining, plan, &span);
  }

  return remaining;
}
