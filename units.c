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
#include <stdio.h>
#include <stdlib.h>

#include "units.h"

double unit_convert(const double value, const unit_type_t value_type,
                    const unit_type_t to_type)
{
  if (value_type == to_type) return value;

  switch (value_type) {
  case UNIT_BYTE:
    if (to_type == UNIT_KILOBYTE) return value / 1000;
    if (to_type == UNIT_MEGABYTE) return value / 1000 / 1000;
    break;

  case UNIT_KILOBYTE:
    if (to_type == UNIT_BYTE)     return value * 1000;
    if (to_type == UNIT_MEGABYTE) return value / 1000;
    break;

  case UNIT_MEGABYTE:
    if (to_type == UNIT_KILOBYTE) return value * 1000;
    if (to_type == UNIT_BYTE)     return value * 1000 * 1000;
    break;

  case UNIT_SECOND:
    if (to_type == UNIT_MINUTE)   return value / 60;
    if (to_type == UNIT_HOUR)     return value / 60 / 60;
    break;

  case UNIT_MINUTE:
    if (to_type == UNIT_SECOND)   return value * 60;
    if (to_type == UNIT_HOUR)     return value / 60;
    break;

  case UNIT_HOUR:
    if (to_type == UNIT_SECOND)   return value * 60 * 60;
    if (to_type == UNIT_MINUTE)   return value * 60;
  }

  return 0;
}

// Find the best conversion for a unit type.
static unit_type_t unit_find_best_type(const double value,
                                       const unit_type_t value_type)
{
  switch (value_type) {
  case UNIT_BYTE:
    if (value >= 1000 * 1000) return UNIT_MEGABYTE;
    if (value >= 1000)        return UNIT_KILOBYTE;
    break;

  case UNIT_KILOBYTE:
    if (value >= 1000)        return UNIT_MEGABYTE;
    if (value < 1)            return UNIT_BYTE;
    break;

  case UNIT_MEGABYTE:
    if (value * 1000 < 1)     return UNIT_BYTE;
    if (value < 1)            return UNIT_KILOBYTE;
    break;

  case UNIT_SECOND:
    if (value >= 60 * 60)     return UNIT_HOUR;
    if (value >= 60)          return UNIT_MINUTE;
    break;

  case UNIT_MINUTE:
    if (value >= 60)          return UNIT_HOUR;
    if (value < 1)            return UNIT_SECOND;
    break;

  case UNIT_HOUR:
    if (value * 60 < 1)       return UNIT_SECOND;
    if (value < 1)            return UNIT_MINUTE;
  }

  return value_type;
}

// Find the label of a unit type.
static const char *unit_find_label(const unit_type_t unit_type) {
  static const char *labels[] = {
    [UNIT_BYTE]      = "byte",
    [UNIT_KILOBYTE]  = "kilobyte",
    [UNIT_MEGABYTE]  = "megabyte",

    [UNIT_SECOND]    = "second",
    [UNIT_MINUTE]    = "minute",
    [UNIT_HOUR]      = "hour",

    [UNIT_INVALID]   = NULL,
  };

  return labels[unit_type];
}

bool unit_convert_best(unit_t *unit, const double value,
                       const unit_type_t value_type)
{
  unit->unit_type = unit_find_best_type(value, value_type);
  unit->amount = unit_convert(value, value_type, unit->unit_type);

  unit->label = unit_find_label(unit->unit_type);
  if (!unit->label) return false;

  snprintf(unit->string, MAX_UNIT_STRING_LENGTH, "%.2f %ss", unit->amount,
                                                             unit->label);

  return true;
}
