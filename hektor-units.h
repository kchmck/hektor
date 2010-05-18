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

#ifndef HEKTOR_UNITS_H
#define HEKTOR_UNITS_H

typedef enum {
  UNIT_BYTE,
  UNIT_KILOBYTE,
  UNIT_MEGABYTE,

  UNIT_SECOND,
  UNIT_MINUTE,
} unit_type_t;

double unit_convert(const double from, const unit_type_t from_type,
                    const unit_type_t to_type);

static inline double bytes_to_megabytes(const double bytes) {
  return unit_convert(bytes, UNIT_BYTE, UNIT_MEGABYTE);
}

#endif
