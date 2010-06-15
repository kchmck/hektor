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

#include <stdint.h>
#include <time.h>

#include "info.h"
#include "modem.h"

typedef struct {
  uint32_t allowed_usage;
  int32_t remaining_usage;

  time_t refill_time;
  time_t refill_timestamp;
} fap_t;

// Initialize a new FAP structure with information parsed from the modem.
void fap_init(fap_t *fap, const page_t info_page);

// Get the "full" amount of the current usage plan in bytes.
static inline uint32_t fap_allowed_usage(const fap_t *fap) {
  return fap->allowed_usage;
}

// Get the remaining usage before the FAP is activated in bytes.
static inline int32_t fap_remaining_usage(const fap_t *fap) {
  return fap->remaining_usage;
}

// Get the time remaining before the FAP is deactivated in seconds.
static inline time_t fap_refill_time(const fap_t *fap) {
  return fap->refill_time;
}

// Get the exact time the FAP will be deactivated as a unix timestamp.
static inline time_t fap_refill_timestamp(const fap_t *fap) {
  return fap->refill_timestamp;
}

// Check if the FAP is active.
static inline bool fap_is_active(const fap_t *fap) {
  return fap_remaining_usage(fap) <= 0;
}

#endif
