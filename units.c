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

double unit_convert(const double from, const unit_type_t from_type,
                    const unit_type_t to_type)
{
  if (from_type == to_type) return from;

  switch (from_type) {
  case UNIT_BYTE:
    if (to_type == UNIT_KILOBYTE) return from / 1000;
    if (to_type == UNIT_MEGABYTE) return from / 1000 / 1000;

  case UNIT_KILOBYTE:
    if (to_type == UNIT_BYTE)     return from * 1000;
    if (to_type == UNIT_MEGABYTE) return from / 1000;

  case UNIT_MEGABYTE:
    if (to_type == UNIT_KILOBYTE) return from * 1000;
    if (to_type == UNIT_BYTE)     return from * 1000 * 1000;

  case UNIT_SECOND:
    if (to_type == UNIT_MINUTE)   return from / 60;

  case UNIT_MINUTE:
    if (to_type == UNIT_SECOND)   return from * 60;
  }

  return from;
}
