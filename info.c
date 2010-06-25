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

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>

#include "common.h"
#include "info.h"
#include "info-page.h"
#include "modem.h"
#include "unit.h"

// Parse the modem type.
static modem_type_t parse_modem_type(const page_t info_page) {
  info_page_value_t modem_type;
  info_page_value_parse(modem_type, info_page, "AdapterType");

  if (strings_are_equal(modem_type, "HN7000S"))
    return MODEM_TYPE_7000;

  if (strings_are_equal(modem_type, "HN9000"))
    return MODEM_TYPE_9000;

  return MODEM_TYPE_INVALID;
}

// Parse the allowed usage limit into bytes.
static uint32_t parse_allowed(const page_t info_page,
                              const modem_type_t modem_type)
{
  if (modem_type == MODEM_TYPE_7000)
    return info_page_number_parse(info_page, "FapAggBucketSize");
  else
    return info_page_number_parse(info_page, "FapPbp2BucketSize");
}

// Parse the remaining usage into bytes.
static uint32_t parse_remaining(const page_t info_page) {
  return min(info_page_number_parse(info_page, "FapAggBucketRemaining"),
             info_page_number_parse(info_page, "FapPbp2BucketRemaining"));
}

// Parse the refill time into seconds.
static time_t parse_refill_time(const page_t info_page) {
  return unit_convert(info_page_number_parse(info_page, "TimeLeftUntilRefill"),
                      UNIT_MINUTE, UNIT_SECOND);
}

// Parse the FAP state.
static fap_state_t parse_fap_state(const page_t info_page) {
  switch (info_page_number_parse(info_page, "FapThrottleState")) {
    case 1: return FAP_STATE_INACTIVE;
    case 2: return FAP_STATE_ACTIVE;
  }

  return FAP_STATE_INVALID;
}

bool info_init(info_t *info, const page_t info_page) {
  info->modem_type = parse_modem_type(info_page);
  if (info->modem_type == MODEM_TYPE_INVALID) return false;

  info->allowed_usage = parse_allowed(info_page, info->modem_type);
  info->remaining_usage = parse_remaining(info_page);

  info->refill_time = parse_refill_time(info_page);
  info->refill_timestamp = time(NULL) + info->refill_time;

  info->fap_state = parse_fap_state(info_page);
  if (info->fap_state == FAP_STATE_INVALID) return false;

  return true;
}
