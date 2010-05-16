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

#include <stdlib.h>
#include <string.h>

#include "hektor-modem.h"
#include "hektor-pep.h"

long pep_extract_value(const char *value_name, const page_t pep_page) {
  // The total length of a pep column, including the name, separating
  // whitespace, and the value. For example:
  //
  // tx_bytes              2147904
  // rx_bytes             130982283
  // ^----------------------------^ is 30 characters
  enum { COLUMN_LENGTH = 30 };

  const char *value_name_match = strstr(pep_page, value_name);
  if (!value_name_match) return 0;

  const size_t value_name_begin = value_name_match - pep_page;
  const size_t value_name_end = value_name_begin + strlen(value_name);

  const size_t value_name_length = value_name_end - value_name_begin;
  const size_t remaining_length = COLUMN_LENGTH - value_name_length;

  // Copy the separating whitespace and pep value into buffer...
  char buffer[remaining_length];
  memcpy(buffer, &pep_page[value_name_end], remaining_length);

  // ... then extract the pep value.
  return atol(buffer);
}
