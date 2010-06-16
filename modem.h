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

#ifndef MODEM_H
#define MODEM_H

#include <stdbool.h>
#include <stddef.h>

// A url type
enum { URL_MAX_LENGTH = 256 + 1};
typedef char url_t[URL_MAX_LENGTH];

// A page type
enum { PAGE_MAX_LENGTH = 2048 + 1 };
typedef char page_t[PAGE_MAX_LENGTH];

// Get the info page's URL.
bool modem_get_info_url(url_t buffer);

// Download a page into a buffer.
size_t modem_fetch_page(page_t buffer, const url_t info_url);

// Initialize global modem stuff.
void modem_global_init(void);

// Free the memory allocated by @modem_global_init.
void modem_global_destroy(void);

#endif
