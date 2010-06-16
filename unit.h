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

#ifndef UNIT_H
#define UNIT_H

#include <stdbool.h>

typedef enum {
  UNIT_BYTE,
  UNIT_KILOBYTE,
  UNIT_MEGABYTE,

  UNIT_SECOND,
  UNIT_MINUTE,
  UNIT_HOUR,
} unit_type_t;

enum { UNIT_STRING_MAX_LENGTH = 32 };
typedef char unit_string_t[UNIT_STRING_MAX_LENGTH];

typedef struct {
  unit_type_t unit_type;

  double amount;
  const char *label;

  unit_string_t string;
} unit_t;

// Convert from one unit to another.
double unit_convert(const double from, const unit_type_t from_type,
                    const unit_type_t to_type);


// Convert a unit to the best representation. For example, 10000 bytes will be
// converted to 10 kilobytes, 120 seconds will be converted to 2 minutes, etc.
bool unit_convert_best(unit_t *unit, const double value,
                       const unit_type_t value_type);

// Get a converted unit's amount: the 100 in 100 bytes.
static inline double unit_amount(const unit_t *unit) {
  return unit->amount;
}

// Get a converted unit's label: the "bytes" in 100 bytes.
static inline const char *unit_label(const unit_t *unit) {
  return unit->label;
}

// Get a converted unit's string: if the amount is 100 and the label is "bytes",
// the string will resemble "100.00 bytes".
static inline const char *unit_string(const unit_t *unit) {
  return unit->string;
}

#endif
