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
#include <stdio.h>
#include <stdlib.h>

#include "hektor-modem.h"
#include "hektor-fap.h"
#include "hektor-units.h"

static bool hektor_main(int argc, char **argv) {
  url_t menu_url;
  modem_get_menu_url(menu_url);

  page_t menu_page;
  if (!modem_fetch_page(menu_url, menu_page)) return false;

  url_t fap_url;
  if (!modem_find_fap_url(menu_page, fap_url)) return false;

  page_t fap_page;
  if (!modem_fetch_page(fap_url, fap_page)) return false;

  printf("%.2f megabytes are remaining.\n",
         unit_convert(fap_get_remaining(fap_page), UNIT_BYTE, UNIT_MEGABYTE));

  return true;
}

int main(int argc, char **argv) {
  return hektor_main(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
