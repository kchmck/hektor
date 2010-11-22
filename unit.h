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

  UNIT_KIBIBYTE,
  UNIT_MEBIBYTE,

  UNIT_KILOBYTE,
  UNIT_MEGABYTE,

  UNIT_SECOND,
  UNIT_MINUTE,
  UNIT_HOUR,

  UNIT_INVALID,
} unit_type_t;

typedef enum {
  UNIT_CLASS_INFO,
  UNIT_CLASS_TIME,

  UNIT_CLASS_INVALID,
} unit_class_t;

typedef enum {
  UNIT_BASE_BINARY,
  UNIT_BASE_SI,

  UNIT_BASE_INVALID,
} unit_base_t;

// Convert @amount from @orig_type to @conv_type.
double unit_convert(const double amount, const unit_type_t orig_type,
                                         const unit_type_t conv_type);

enum { UNIT_STRING_LENGTH = 32 + 1 };
typedef char unit_string_t[UNIT_STRING_LENGTH];

typedef struct {
  unit_class_t unit_class;
  unit_base_t unit_base;

  unit_type_t orig_type;
  double orig_amount;

  unit_type_t conv_type;
  double conv_amount;

  const char *label;
  unit_string_t string;
} unit_conv_t;

// Initialize @conv with @amount of @type.
void unit_conv_init(unit_conv_t *conv, const unit_type_t type,
                    const double amount);

// Set the @base of a unit conversion.
static inline void unit_conv_set_base(unit_conv_t *conv, const unit_base_t base)
{
  conv->unit_base = base;
}

// Set the @type of the original unit.
static inline void unit_conv_set_type(unit_conv_t *conv, const unit_type_t type)
{
  conv->orig_type = type;
}

// Set the @amount of the original unit.
static inline void unit_conv_set_amount(unit_conv_t *conv, const double amount)
{
  conv->orig_amount = amount;
}

// Perform the unit conversion.
bool unit_conv_calculate(unit_conv_t *conv);

// Get a converted unit's amount: the 100 in 100 bytes.
static inline double unit_conv_amount(const unit_conv_t *conv) {
  return conv->conv_amount;
}

// Get a converted unit's label: the "bytes" in 100 bytes.
static inline const char *unit_conv_label(const unit_conv_t *conv) {
  return conv->label;
}

// Get a converted unit's string: if the amount is 100 and the label is "bytes",
// the string will resemble "100.0 bytes".
static inline const char *unit_conv_string(const unit_conv_t *conv) {
  return conv->string;
}

#endif
