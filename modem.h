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

// A url type
enum { MAX_URL_LENGTH = 255 };
typedef char url_t[MAX_URL_LENGTH];

// A page type
enum { MAX_PAGE_SIZE = 1024 * 64 };
typedef char page_t[MAX_PAGE_SIZE];

// Get the menu page's url
bool modem_get_menu_url(url_t buffer);

// Get the fap page's url.
bool modem_get_fap_url(url_t buffer, const page_t menu_page);

// Download a page into a buffer.
size_t modem_fetch_page(page_t buffer, const url_t url);

#endif
