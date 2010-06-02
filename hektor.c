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

#include "fap.h"
#include "modem.h"
#include "units.h"

static void hektor_show_remaining(const long remaining) {
  printf("%.2f megabytes are remaining.\n",
         unit_convert(remaining, UNIT_BYTE, UNIT_MEGABYTE));
}

static void hektor_show_refill_time(const page_t fap_page) {
  unit_t refill_time;
  unit_convert_smart(&refill_time, fap_get_refill_time(fap_page), UNIT_SECOND);

  printf("%s until the FAP is deactivated.\n", unit_string(&refill_time));
}

static bool hektor_main(int argc, char **argv) {
  url_t menu_url;
  if (!modem_get_menu_url(menu_url)) return false;

  page_t menu_page;
  if (!modem_fetch_page(menu_page, menu_url)) return false;

  url_t fap_url;
  if (!modem_get_fap_url(fap_url, menu_page)) return false;

  page_t fap_page;
  if (!modem_fetch_page(fap_page, fap_url)) return false;

  const long remaining = fap_get_remaining(fap_page);
  
  if (fap_is_active(remaining)) hektor_show_refill_time(fap_page);
  else                          hektor_show_remaining(remaining);

  return true;
}

int main(int argc, char **argv) {
  return hektor_main(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
