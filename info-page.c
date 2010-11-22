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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "info-page.h"
#include "modem.h"

// The info page is laid out like:
//
//    ┌───────────────────┬─ value_name
//    FapAggBucketRemaining=118914707\n
//              SEPARATOR ─┘└───────┴─ string copied into value_buffer
//
// The variables below end up like:
//
//    ┌─ value_name_begin  ┌─ sep_begin
//    FapAggBucketRemaining=118914707\n
//            value_begin  ─┘        └─ value_end
//
bool info_page_value_parse(info_page_value_t value_buffer,
                           const page_t info_page,
                           const char *value_name)
{
  static const char SEPARATOR[] = "=";
  enum { SEPARATOR_LENGTH = string_length(SEPARATOR) };

  const char *value_name_begin = strstr(info_page, value_name);
  if (!value_name_begin) return false;

  const char *sep_begin = strstr(value_name_begin, SEPARATOR);
  if (!sep_begin) return false;

  const char *value_begin = sep_begin + SEPARATOR_LENGTH;
  const char *value_end = strchr(value_begin, '\n');
  if (!value_end) return false;

  const ptrdiff_t value_length = value_end - value_begin;
  const ptrdiff_t value_length_with_null = value_length + 1;

  return string_copy(value_begin, value_buffer,
                     min(value_length_with_null, INFO_PAGE_VALUE_LENGTH));
}

uint32_t info_page_number_parse(const page_t info_page, const char *value_name)
{
  info_page_value_t value;
  info_page_value_parse(value, info_page, value_name);

  return atoi(value);
}
