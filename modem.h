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

enum { URL_LENGTH = 256 };
typedef char url_t[URL_LENGTH];

enum { PAGE_LENGTH = 2048 };
typedef char page_t[PAGE_LENGTH];

// Initialize global modem stuff.
void modem_global_init(void);

// Free the memory allocated by @modem_global_init.
void modem_global_destroy(void);

// Get the info page URL.
bool modem_build_info_url(url_t buffer);

// Get the restart URL.
bool modem_build_restart_url(url_t buffer);

// Restart the modem.
bool modem_restart(const url_t restart_url);

// Download the page at @url into @buffer.
size_t modem_fetch_page(page_t buffer, const url_t url);

#endif
