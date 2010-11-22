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

// Find the class of a @unit_type.
static unit_class_t find_class(const unit_type_t unit_type) {
  switch (unit_type) {
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

// Convert between information units.
static double convert_info(const double amount, const unit_type_t orig_type,
                                                const unit_type_t conv_type)
{
  switch (orig_type) {
    case UNIT_BYTE:
      if (conv_type == UNIT_KIBIBYTE) return amount / 1024;
      if (conv_type == UNIT_MEBIBYTE) return amount / 1024 / 1024;
      if (conv_type == UNIT_KILOBYTE) return amount / 1000;
      if (conv_type == UNIT_MEGABYTE) return amount / 1000 / 1000;
      break;

    case UNIT_KIBIBYTE:
      if (conv_type == UNIT_BYTE)     return amount * 1024;
      if (conv_type == UNIT_MEBIBYTE) return amount / 1024;
      break;

    case UNIT_MEBIBYTE:
      if (conv_type == UNIT_BYTE)     return amount * 1024 * 1024;
      if (conv_type == UNIT_KIBIBYTE) return amount * 1024;
      break;

    case UNIT_KILOBYTE:
      if (conv_type == UNIT_BYTE)     return amount * 1000;
      if (conv_type == UNIT_MEGABYTE) return amount / 1000;
      break;

    case UNIT_MEGABYTE:
      if (conv_type == UNIT_KILOBYTE) return amount * 1000;
      if (conv_type == UNIT_BYTE)     return amount * 1000 * 1000;
  }

  return 0;
}

// Convert between time units.
static double convert_time(const double amount, const unit_type_t orig_type,
                                                const unit_type_t conv_type)
{
  switch (orig_type) {
    case UNIT_SECOND:
      if (conv_type == UNIT_MINUTE)   return amount / 60;
      if (conv_type == UNIT_HOUR)     return amount / 60 / 60;
      break;

    case UNIT_MINUTE:
      if (conv_type == UNIT_SECOND)   return amount * 60;
      if (conv_type == UNIT_HOUR)     return amount / 60;
      break;

    case UNIT_HOUR:
      if (conv_type == UNIT_SECOND)   return amount * 60 * 60;
      if (conv_type == UNIT_MINUTE)   return amount * 60;
  }

  return 0;
}

double unit_convert(const double amount, const unit_type_t orig_type,
                                         const unit_type_t conv_type)
{
  if (orig_type == conv_type)
    return amount;

  switch (find_class(orig_type)) {
    case UNIT_CLASS_INFO:
      return convert_info(amount, orig_type, conv_type);

    case UNIT_CLASS_TIME:
      return convert_time(amount, orig_type, conv_type);
  }

  return 0;
}

// Find the best time unit for an amount.
static unit_type_t find_best_time_type(const unit_conv_t *conv) {
  const double amount = conv->orig_amount;

  switch (conv->orig_type) {
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
  }

  return conv->orig_type;
}

// Find the best binary unit for an amount.
static unit_type_t find_best_binary_type(const double amount,
                                         const unit_type_t orig_type)
{
  switch (orig_type) {
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
  }

  return orig_type;
}

// Find the best SI unit for an amount.
static unit_type_t find_best_si_type(const double amount,
                                     const unit_type_t orig_type)
{
  switch (orig_type) {
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
  }

  return orig_type;
}

// Find the best info unit for an amount and a base.
static unit_type_t find_best_info_type(const unit_conv_t *conv) {
  switch (conv->unit_base) {
    case UNIT_BASE_BINARY:
      return find_best_binary_type(conv->orig_amount, conv->orig_type);

    case UNIT_BASE_SI:
      return find_best_si_type(conv->orig_amount, conv->orig_type);
  }

  return UNIT_BASE_INVALID;
}

// Find the best conversion for a unit type.
static unit_type_t find_best_type(const unit_conv_t *conv)
{
  switch (conv->unit_class) {
    case UNIT_CLASS_TIME:
      return find_best_time_type(conv);

    case UNIT_CLASS_INFO:
      return find_best_info_type(conv);
  }

  return UNIT_CLASS_INVALID;
}

// Find the label for a @unit_type.
static const char *find_label(const unit_type_t unit_type) {
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

  return labels[unit_type];
}

void unit_conv_init(unit_conv_t *conv, const unit_type_t type,
                    const double amount)
{
  unit_conv_set_amount(conv, amount);
  unit_conv_set_type(conv, type);
  unit_conv_set_base(conv, UNIT_BASE_SI);
}

bool unit_conv_calculate(unit_conv_t *conv) {
  conv->unit_class = find_class(conv->orig_type);
  if (conv->unit_class == UNIT_CLASS_INVALID) return false;

  conv->conv_type = find_best_type(conv);
  if (conv->conv_type == UNIT_INVALID) return false;

  conv->conv_amount = unit_convert(conv->orig_amount, conv->orig_type,
                                                      conv->conv_type);

  conv->label = find_label(conv->conv_type);
  if (!conv->label) return false;

  snprintf(conv->string, UNIT_STRING_LENGTH, "%.1f %ss", conv->conv_amount,
                                                         conv->label);

  return true;
}
