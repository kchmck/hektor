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

#include <string.h>
#include "hektor-plan.h"

enum { PLANS_LENGTH = 6 };

static const struct {
  const char *const plan_name;
  const plan_id_t plan_id;
} plan_ids[PLANS_LENGTH] = {
  {"home",          PLAN_HOME},
  {"pro",           PLAN_PRO},
  {"pro-plus",      PLAN_PRO_PLUS},
  {"elite",         PLAN_ELITE},
  {"elite-plus",    PLAN_ELITE_PLUS},
  {"elite-premium", PLAN_ELITE_PREMIUM}
};

static const long plan_thresholds[PLANS_LENGTH] = {
  [PLAN_HOME]          = 200000000,
  [PLAN_PRO]           = 300000000,
  [PLAN_PRO_PLUS]      = 425000000,
  [PLAN_ELITE]         = 500000000,
  [PLAN_ELITE_PLUS]    = 500000000,
  [PLAN_ELITE_PREMIUM] = 500000000
};

// Get the bandwidth threshold of a plan.
static inline long plan_find_threshold(const plan_id_t plan_id) {
  return plan_thresholds[plan_id];
}

// Get the refill rate of a plan in bytes per second.
static inline double plan_calc_refill_rate(const long threshold) {
  return (double)threshold / 24 / 60 / 60;
}

// Match a plan name to an id.
static plan_id_t plan_find_id(const char *const plan_name) {
  for (int i = 0; i < PLANS_LENGTH; i += 1)
    if (strcmp(plan_name, plan_ids[i].plan_name) == 0)
      return plan_ids[i].plan_id;

  return PLAN_INVALID;
}

bool plan_load(const char *const plan_name, plan_t *const plan) {
  plan->plan_id = plan_find_id(plan_name);
  if (plan->plan_id == PLAN_INVALID) return false;

  plan->plan_name = plan_name;
  plan->threshold = plan_find_threshold(plan->plan_id);
  plan->refill_rate = plan_calc_refill_rate(plan->threshold);

  return true;
}
