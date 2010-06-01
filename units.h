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

#ifndef UNITS_H
#define UNITS_H

#include <stdbool.h>

typedef enum {
  UNIT_BYTE,
  UNIT_KILOBYTE,
  UNIT_MEGABYTE,

  UNIT_SECOND,
  UNIT_MINUTE,
  UNIT_HOUR,

  UNIT_INVALID,
} unit_type_t;

enum { MAX_UNIT_STRING_LENGTH = 32 };
typedef char unit_string_t[MAX_UNIT_STRING_LENGTH];

typedef struct {
  unit_type_t unit_type;
  double amount;
  const char *label;

  unit_string_t string;
} unit_t;

// Convert from one unit to another.
double unit_convert(const double from, const unit_type_t from_type,
                    const unit_type_t to_type);


bool unit_convert_smart(unit_t *unit, const double value,
                        const unit_type_t value_type);

static inline double unit_amount(const unit_t *unit) {
  return unit->amount;
}

static inline const char *unit_label(const unit_t *unit) {
  return unit->label;
}

static inline const char *unit_string(const unit_t *unit) {
  return unit->string;
}

#endif
