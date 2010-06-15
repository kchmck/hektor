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

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "modem.h"
#include "info.h"

#include "fap.h"

// Parse the usage limit into bytes.
static inline uint32_t fap_parse_allowed(const page_t info_page) {
  return info_integer_parse(info_page, "FapAggBucketSize");
}

// Parse the usage remaining into bytes.
static inline int32_t fap_parse_remaining(const page_t info_page) {
  return info_integer_parse(info_page, "FapAggBucketRemaining");
}

// Parse the refill time into seconds.
static inline time_t fap_parse_refill_time(const page_t info_page) {
  return info_integer_parse(info_page, "TimeLeftUntilRefill");
}

void fap_init(fap_t *fap, const page_t info_page) {
  fap->allowed_usage = fap_parse_allowed(info_page);
  fap->remaining_usage = fap_parse_remaining(info_page);
  fap->refill_time = fap_parse_refill_time(info_page);
  fap->refill_timestamp = time(NULL) + fap->refill_time;
}
