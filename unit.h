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
} unit_t;

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

// Convert @amount of @orig_type to @conv_type.
//
//   @return: a converted amount or 0.0 on error.
//
//   unit_convert(1, UNIT_BYTE, UNIT_KILOBYTE)    -> 1000.0
//   unit_convert(1, UNIT_BYTE, UNIT_KIBIBYTE)    -> 1024.0
//   unit_convert(1000, UNIT_KILOBYTE, UNIT_BYTE) -> 1.0
//   unit_convert(1, UNIT_HOUR, UNIT_SECOND)      -> 3600.0
//
double unit_convert(const double amount, const unit_t orig_unit,
                                         const unit_t conv_unit);

enum { UNIT_STRING_LENGTH = 32 };
typedef char unit_string_t[UNIT_STRING_LENGTH];

typedef struct {
  unit_t orig_unit;
  unit_class_t orig_class;
  unit_base_t orig_base;
  double orig_amount;

  unit_t conv_unit;
  double conv_amount;

  const char *label;
  unit_string_t string;
} unit_conv_t;

// Initialize @conv with @amount of @unit. The conversion base is initialized to
// UNIT_BASE_SI, but can be changed with @unit_conv_set_base.
//
//   unit_conv_t conv;
//   unit_conv_init(&conv, UNIT_BYTE, 1200);
//
void unit_conv_init(unit_conv_t *conv, const unit_t unit,
                                       const double amount);

// Set the conversion base of @conv.
static inline void unit_conv_set_base(unit_conv_t *conv, const unit_base_t base)
{
  conv->orig_base = base;
}

// Convert @conv to the "best" unit.
//
//   @return: true on success and false otherwise.
//
//   unit_conv_calculate(&conv) -> true
//
bool unit_conv_calculate(unit_conv_t *conv);

// Get a converted unit's amount.
//
//   unit_conv_amount(&conv) -> 1.2
//
static inline double unit_conv_amount(const unit_conv_t *conv) {
  return conv->conv_amount;
}

// Get a converted unit's label.
//
//   unit_conv_label(&conv) -> "kilobytes"
//
static inline const char *unit_conv_label(const unit_conv_t *conv) {
  return conv->label;
}

// Get a converted unit's string.
//
//   unit_conv_string(&conv) -> "1.20 kilobytes"
//
static inline const char *unit_conv_string(const unit_conv_t *conv) {
  return conv->string;
}

#endif
