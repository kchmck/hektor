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

#ifndef INFO_H
#define INFO_H

#include <stdbool.h>
#include <stdint.h>

#include "modem.h"

// An info value type
enum { INFO_VALUE_MAX_LENGTH = 32 + 1 };
typedef char info_value_t[INFO_VALUE_MAX_LENGTH];

// Get a string value from the info page.
bool info_value_parse(info_value_t value_buffer, const page_t info_page,
                      const char *value_name);

// Get an integer value from the info page.
int32_t info_integer_parse(const page_t info_page, const char *value_name);

#endif
