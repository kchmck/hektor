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
// A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along with
// Hektor.  If not, see <http://www.gnu.org/licenses/>.

#include "hektor-plan.h"

bool plan_load(plan_t *plan, const plan_id_t plan_id) {
  plan->threshold = plan_threshold(plan_id);
  if (!plan->threshold) return false;

  plan->refill_rate = plan_refill_rate(plan->threshold);

  return true;
}
