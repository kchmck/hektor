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

#ifndef HEKTOR_PLAN_H
#define HEKTOR_PLAN_H

#include <stdbool.h>

typedef enum {
  PLAN_HOME,
  PLAN_PRO,
  PLAN_PRO_PLUS,
  PLAN_ELITE,
  PLAN_ELITE_PLUS,
  PLAN_ELITE_PREMIUM,

  PLAN_INVALID
} plan_id_t;

typedef struct {
  plan_id_t plan_id;
  const char *plan_name;

  // The rolling, 24-hour bandwidth threshold of a plan in bytes
  long threshold;
  // The amount of bytes refilled per second
  double refill_rate;
} plan_t;

// Load a new plan.
bool plan_load(const char *const plan_name, plan_t *const plan);

#endif
