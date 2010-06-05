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

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "modem.h"
#include "units.h"

#include "fap.h"

// Get a value from the FAP page. The FAP page is laid out like:
//
//   ┌───────────────────────────────┬─ value_name
//   Unthrottle data in current window : 49499 KB
//                         SEPARATOR ─┴─┘└───┴─ integer returned
//
// The variables below end up like:
//
//   ┌─ value_name_begin              ┌─ sep_begin
//   Unthrottle data in current window : 49499 KB
//                                       └─ value_begin
//
// atoi does the rest.
static int fap_parse_value(const char *value_name, const page_t fap_page) {
  static const char SEPARATOR[] = " : ";
  enum { SEPARATOR_LENGTH = sizeof(SEPARATOR) - 1 };

  const char *value_name_begin = strstr(fap_page, value_name);
  if (!value_name_begin) return 0;

  const char *sep_begin = strstr(value_name_begin, SEPARATOR);
  if (!sep_begin) return 0;

  const char *value_begin = sep_begin + SEPARATOR_LENGTH;

  return atoi(value_begin);
}

// Parse the usage limit into bytes.
static long fap_parse_limit(const page_t fap_page) {
  return unit_convert(fap_parse_value("User FAP Limit", fap_page),
                      UNIT_KILOBYTE, UNIT_BYTE);
}

// Parse the usage remaining into bytes.
static long fap_parse_remaining(const page_t fap_page) {
  return unit_convert(fap_parse_value("Unthrottle data in current window",
                                      fap_page),
                      UNIT_KILOBYTE, UNIT_BYTE);
}

// Parse the refill time into seconds.
static int fap_parse_refill_time(const page_t fap_page) {
  return unit_convert(fap_parse_value("FAP count down timer", fap_page),
                      UNIT_MINUTE, UNIT_SECOND);
}

void fap_init(fap_t *fap, const page_t fap_page) {
  fap->usage_limit = fap_parse_limit(fap_page);
  fap->usage_remaining = fap_parse_remaining(fap_page);
  fap->remaining_refill_time = fap_parse_refill_time(fap_page);
}

time_t fap_exact_refill_time(const fap_t *fap) {
  const time_t refill_seconds = fap_remaining_refill_time(fap);
  const time_t now_timestamp = time(NULL);

  return now_timestamp + refill_seconds;
}
