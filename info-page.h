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

#ifndef INFO_PAGE_H
#define INFO_PAGE_H

#include <stdbool.h>
#include <stdint.h>

#include "modem.h"

enum { INFO_PAGE_VALUE_MAX_LENGTH = 32 + 1 };
typedef char info_page_value_t[INFO_PAGE_VALUE_MAX_LENGTH];

// Get a string value from the info page.
bool info_page_value_parse(info_page_value_t value_buffer,
                           const page_t info_page,
                           const char *value_name);

// Get an integer value from the info page.
uint32_t info_page_number_parse(const page_t info_page, const char *value_name);

#endif
