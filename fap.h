// Copyright 2010 Mick Koch <kchmck@gmail.com>
//
// This file is part of hektor.
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
// hektor. If not, see <http://www.gnu.org/licenses/>.

#ifndef FAP_H
#define FAP_H

#include <time.h>
#include "modem.h"

typedef struct {
  long limit;
  long remaining;
  time_t remaining_refill_time;
} fap_t;

void fap_init(fap_t *fap, const page_t fap_page);

static inline long fap_usage_limit(const fap_t *fap) {
  return fap->limit;
}

static inline long fap_usage_remaining(const fap_t *fap) {
  return fap->remaining;
}

static inline time_t fap_remaining_refill_time(const fap_t *fap) {
  return fap->remaining_refill_time;
}

time_t fap_exact_refill_time(const fap_t *fap);

// Check if the FAP is active.
static inline bool fap_is_active(const fap_t *fap) {
  return fap_usage_remaining(fap) <= 0;
}

#endif
