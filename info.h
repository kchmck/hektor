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

#ifndef INFO_H
#define INFO_H

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "modem.h"

typedef enum {
  MODEM_TYPE_7000,
  MODEM_TYPE_9000,

  MODEM_TYPE_INVALID,
} modem_type_t;

typedef enum {
  FAP_STATE_ACTIVE,
  FAP_STATE_INACTIVE,

  FAP_STATE_INVALID,
} fap_state_t;

typedef struct {
  modem_type_t modem_type;

  uint32_t allowed_usage;
  uint32_t remaining_usage;

  time_t refill_time;
  time_t refill_timestamp;

  fap_state_t fap_state;
} info_t;

// Initialize a new @info structure with information parsed from the modem.
bool info_init(info_t *info, const page_t info_page);

// Get the modem type.
static inline modem_type_t info_modem_type(const info_t *info) {
  return info->modem_type;
}

// Get the "full" amount of the current usage plan in bytes.
static inline uint32_t info_allowed_usage(const info_t *info) {
  return info->allowed_usage;
}

// Get the remaining usage in bytes.
static inline uint32_t info_remaining_usage(const info_t *info) {
  return info->remaining_usage;
}

// Get the time remaining before the FAP is deactivated in seconds.
static inline time_t info_refill_time(const info_t *info) {
  return info->refill_time;
}

// Get the exact time the FAP will be deactivated as a unix timestamp.
static inline time_t info_refill_timestamp(const info_t *info) {
  return info->refill_timestamp;
}

// Get the FAP state.
static inline fap_state_t info_fap_state(const info_t *info) {
  return info->fap_state;
}

#endif
