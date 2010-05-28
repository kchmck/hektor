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

#include "units.h"

double unit_convert(const double value, const unit_type_t value_type,
                    const unit_type_t to_type)
{
  if (value_type == to_type) return value;

  switch (value_type) {
  case UNIT_BYTE:
    if (to_type == UNIT_KILOBYTE) return value / 1000;
    if (to_type == UNIT_MEGABYTE) return value / 1000 / 1000;

  case UNIT_KILOBYTE:
    if (to_type == UNIT_BYTE)     return value * 1000;
    if (to_type == UNIT_MEGABYTE) return value / 1000;

  case UNIT_MEGABYTE:
    if (to_type == UNIT_KILOBYTE) return value * 1000;
    if (to_type == UNIT_BYTE)     return value * 1000 * 1000;

  case UNIT_SECOND:
    if (to_type == UNIT_MINUTE)   return value / 60;
    if (to_type == UNIT_HOUR)     return value / 60 / 60;

  case UNIT_MINUTE:
    if (to_type == UNIT_SECOND)   return value * 60;
    if (to_type == UNIT_HOUR)     return value / 60;

  case UNIT_HOUR:
    if (to_type == UNIT_SECOND)   return value * 60 * 60;
    if (to_type == UNIT_MINUTE)   return value * 60;
  }

  return 0;
}
