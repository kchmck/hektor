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

#include "hektor-modem.h"
#include "hektor-units.h"

#include "hektor-fap.h"

// Get a value from the fap page. The page is laid out like:
//
//   |-------------------------------| is the value name
//   Unthrottle data in current window : 49499 KB 
//                                       |---| is the value
//
// The value name and value are separated by ' : '.
static int fap_get_value(const char *value_name, const page_t fap_page) {
  static const char SEPARATOR[] = " : ";
  enum { SEPARATOR_LENGTH = sizeof(SEPARATOR) - 1 };

  const char *value_name_begin = strstr(fap_page, value_name);
  if (!value_name_begin) return 0;

  const char *sep_begin = strstr(value_name_begin, SEPARATOR);
  if (!sep_begin) return 0;

  const char *value_begin = sep_begin + SEPARATOR_LENGTH;

  return atoi(value_begin);
}

long fap_get_limit(const page_t fap_page) {
  return unit_convert(fap_get_value("User FAP Limit", fap_page),
                      UNIT_KILOBYTE, UNIT_BYTE);
}

long fap_get_remaining(const page_t fap_page) {
  return unit_convert(fap_get_value("Unthrottle data in current window",
                                    fap_page),
                      UNIT_KILOBYTE, UNIT_BYTE);
}

int fap_get_refill_time(const page_t fap_page) {
  return unit_convert(fap_get_value("FAP count down timer", fap_page),
                      UNIT_MINUTE, UNIT_SECOND);
}
