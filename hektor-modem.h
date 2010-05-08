// Copyright 2010 Mick Koch <kchmck@gmail.com>
//
// This file is part of Hektor.
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
// Hektor. If not, see <http://www.gnu.org/licenses/>.

#ifndef HEKTOR_MODEM_H
#define HEKTOR_MODEM_H

#include <stdbool.h>

#include "hektor-common.h"
#include "hektor-path.h"

// A url type
enum { MAX_URL_LENGTH = MAX_PATH_LENGTH };
typedef char url_t[MAX_URL_LENGTH];

// A page type
enum { MAX_PAGE_SIZE = 1024 * 64 };
typedef char page_t[MAX_PAGE_SIZE];

// Dynamically find the url of a page by its page title.
bool modem_find_url(const char *page_title, const page_t menu_page,
                    url_t url_buffer);

// Get the pep page's url.
static inline bool modem_find_pep_url(const page_t menu_page, url_t url_buffer) {
  return modem_find_url("ALL BCB Stats", menu_page, url_buffer);
}

// Download a page into a buffer.
size_t modem_fetch_page(const url_t url, page_t buffer);

// Download the menu page.
static inline size_t modem_fetch_menu_page(page_t buffer) {
  return modem_fetch_page("/fs/scripts/cat_menu.js", buffer);
}

#endif
