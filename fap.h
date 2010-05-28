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

#ifndef FAP_H
#define FAP_H

#include "modem.h"

// Get the bandwidth limit in bytes.
long fap_get_limit(const page_t fap_page);

// Get the remaining usage in bytes.
long fap_get_remaining(const page_t fap_page);

// Get the time until the fap is deactivated in seconds.
int fap_get_refill_time(const page_t fap_page);

// Check if the FAP is active.
bool fap_is_active(const long remaining);

#endif
