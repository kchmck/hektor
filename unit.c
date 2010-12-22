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

#include "unit.h"

// Find the class of @unit.
//
//   unit_class(UNIT_BYTE)   -> UNIT_CLASS_INFO
//   unit_class(UNIT_SECOND) -> UNIT_CLASS_TIME
//
static unit_class_t unit_class(const unit_t unit) {
  switch (unit) {
    case UNIT_BYTE:
    case UNIT_KIBIBYTE:
    case UNIT_MEBIBYTE:
    case UNIT_KILOBYTE:
    case UNIT_MEGABYTE:
      return UNIT_CLASS_INFO;

    case UNIT_SECOND:
    case UNIT_MINUTE:
    case UNIT_HOUR:
      return UNIT_CLASS_TIME;
  }

  return UNIT_CLASS_INVALID;
}

// Convert @amount of @orig_unit to @conv_unit.
static double unit_convert_info(const double amount, const unit_t orig_unit,
                                                     const unit_t conv_unit)
{
  switch (orig_unit) {
    case UNIT_BYTE:
      switch (conv_unit) {
        case UNIT_KIBIBYTE: return amount / 1024;
        case UNIT_MEBIBYTE: return amount / 1024 / 1024;
        case UNIT_KILOBYTE: return amount / 1000;
        case UNIT_MEGABYTE: return amount / 1000 / 1000;
      }
    break;

    case UNIT_KIBIBYTE:
      switch (conv_unit) {
        case UNIT_BYTE:     return amount * 1024;
        case UNIT_MEBIBYTE: return amount / 1024;
      }
    break;

    case UNIT_MEBIBYTE:
      switch (conv_unit) {
        case UNIT_BYTE:     return amount * 1024 * 1024;
        case UNIT_KIBIBYTE: return amount * 1024;
      }
    break;

    case UNIT_KILOBYTE:
      switch (conv_unit) {
        case UNIT_BYTE:     return amount * 1000;
        case UNIT_MEGABYTE: return amount / 1000;
      }
    break;

    case UNIT_MEGABYTE:
      switch (conv_unit) {
        case UNIT_KILOBYTE: return amount * 1000;
        case UNIT_BYTE:     return amount * 1000 * 1000;
      }
    break;
  }

  return 0;
}

// Convert @amount of @orig_unit to @conv_unit.
static double unit_convert_time(const double amount, const unit_t orig_unit,
                                                     const unit_t conv_unit)
{
  switch (orig_unit) {
    case UNIT_SECOND:
      switch (conv_unit) {
        case UNIT_MINUTE: return amount / 60;
        case UNIT_HOUR:   return amount / 60 / 60;
      }
    break;

    case UNIT_MINUTE:
      switch (conv_unit) {
        case UNIT_SECOND: return amount * 60;
        case UNIT_HOUR:   return amount / 60;
      }
    break;

    case UNIT_HOUR:
      switch (conv_unit) {
        case UNIT_SECOND: return amount * 60 * 60;
        case UNIT_MINUTE: return amount * 60;
      }
    break;
  }

  return 0;
}

double unit_convert(const double amount, const unit_t orig_unit,
                                         const unit_t conv_unit)
{
  if (orig_unit == conv_unit)
    return amount;

  switch (unit_class(orig_unit)) {
    case UNIT_CLASS_INFO:
      return unit_convert_info(amount, orig_unit, conv_unit);

    case UNIT_CLASS_TIME:
      return unit_convert_time(amount, orig_unit, conv_unit);
  }

  return 0;
}

// Find the best time unit for @amount of @unit.
static unit_t unit_best_time(const double amount, const unit_t unit) {
  switch (unit) {
    case UNIT_SECOND:
      if (amount >= 60 * 60) return UNIT_HOUR;
      if (amount >= 60)      return UNIT_MINUTE;
    break;

    case UNIT_MINUTE:
      if (amount >= 60)      return UNIT_HOUR;
      if (amount < 1)        return UNIT_SECOND;
    break;

    case UNIT_HOUR:
      if (amount * 60 < 1)   return UNIT_SECOND;
      if (amount < 1)        return UNIT_MINUTE;
    break;
  }

  return unit;
}

// Find the best binary unit for @amount of @unit.
static unit_t unit_best_binary(const double amount, const unit_t unit) {
  switch (unit) {
    case UNIT_BYTE:
      if (amount >= 1024 * 1024) return UNIT_MEBIBYTE;
      if (amount >= 1024)        return UNIT_KIBIBYTE;
    break;

    case UNIT_KIBIBYTE:
      if (amount >= 1024)        return UNIT_MEBIBYTE;
      if (amount < 1)            return UNIT_BYTE;
    break;

    case UNIT_MEBIBYTE:
      if (amount * 1024 < 1)     return UNIT_BYTE;
      if (amount < 1)            return UNIT_KIBIBYTE;
    break;
  }

  return unit;
}

// Find the best SI unit for @amount of @unit.
static unit_t unit_best_si(const double amount, const unit_t unit) {
  switch (unit) {
    case UNIT_BYTE:
      if (amount >= 1000 * 1000) return UNIT_MEGABYTE;
      if (amount >= 1000)        return UNIT_KILOBYTE;
    break;

    case UNIT_KILOBYTE:
      if (amount >= 1000)        return UNIT_MEGABYTE;
      if (amount < 1)            return UNIT_BYTE;
    break;

    case UNIT_MEGABYTE:
      if (amount * 1000 < 1)     return UNIT_BYTE;
      if (amount < 1)            return UNIT_KILOBYTE;
    break;
  }

  return unit;
}

// Find the best info unit for an amount and a base.
static unit_t unit_best_info(const double amount, const unit_t unit,
                             const unit_base_t base)
{
  switch (base) {
    case UNIT_BASE_BINARY: return unit_best_binary(amount, unit);
    case UNIT_BASE_SI:     return unit_best_si(amount, unit);
  }

  return UNIT_INVALID;
}

// Find the label for a @unit_unit.
static const char *unit_label(const unit_t unit) {
  static const char *labels[] = {
    [UNIT_BYTE]     = "byte",

    [UNIT_KIBIBYTE] = "kibibyte",
    [UNIT_MEBIBYTE] = "mebibyte",

    [UNIT_KILOBYTE] = "kilobyte",
    [UNIT_MEGABYTE] = "megabyte",

    [UNIT_SECOND]   = "second",
    [UNIT_MINUTE]   = "minute",
    [UNIT_HOUR]     = "hour",

    [UNIT_INVALID]  = NULL,
  };

  return labels[unit];
}

void unit_conv_init(unit_conv_t *conv, const unit_t unit,
                                       const double amount)
{
  conv->orig_unit = unit;
  conv->orig_base = UNIT_BASE_SI;
  conv->orig_amount = amount;
}

// Find the best conversion for @orig_amount of @orig_unit.
static unit_t unit_conv_best_unit(const unit_conv_t *conv) {
  switch (conv->orig_class) {
    case UNIT_CLASS_TIME:
      return unit_best_time(conv->orig_amount, conv->orig_unit);

    case UNIT_CLASS_INFO:
      return unit_best_info(conv->orig_amount, conv->orig_unit,
                            conv->orig_base);
  }

  return UNIT_INVALID;
}

bool unit_conv_calculate(unit_conv_t *conv) {
  conv->orig_class = unit_class(conv->orig_unit);
  if (conv->orig_class == UNIT_CLASS_INVALID) return false;

  conv->conv_unit = unit_conv_best_unit(conv);
  if (conv->conv_unit == UNIT_INVALID) return false;

  conv->conv_amount = unit_convert(conv->orig_amount, conv->orig_unit,
                                                      conv->conv_unit);

  conv->label = unit_label(conv->conv_unit);
  if (!conv->label) return false;

  snprintf(conv->string, UNIT_STRING_LENGTH, "%.1f %ss", conv->conv_amount,
                                                         conv->label);

  return true;
}
