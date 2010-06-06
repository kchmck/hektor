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
#include <time.h>

#include "fap.h"
#include "modem.h"
#include "unit.h"

static void hektor_show_refill_time(const fap_t *fap) {
  enum { MAX_REFILL_TIME_LENGTH = 32 };

  const time_t remaining_time = fap_remaining_refill_time(fap);
  const time_t exact_time = fap_exact_refill_time(fap);

  unit_t refill_time_unit;
  unit_convert_best(&refill_time_unit, remaining_time, UNIT_SECOND);

  char refill_time_string[MAX_REFILL_TIME_LENGTH];
  strftime(refill_time_string, MAX_REFILL_TIME_LENGTH, "%I:%M %p %A",
           localtime(&exact_time));

  printf("The FAP will be deactivated in %s, at %s.\n",
         unit_string(&refill_time_unit), refill_time_string);
}

static void hektor_show_remaining(const fap_t *fap) {
  unit_t remaining;
  unit_convert_best(&remaining, fap_usage_remaining(fap), UNIT_BYTE);

  printf("%s are remaining.\n", unit_string(&remaining));
}

static bool hektor_error_fetching_page(const url_t url) {
  printf("An error occured while retrieving the page at ‘%s’.\n", url);

  return false;
}

static bool hektor_error_finding_url(const char *page_name) {
  printf("An error occured while trying to find the %s page’s URL.\n",
         page_name);

  return false;
}

static bool hektor_main(int argc, char **argv) {
  url_t menu_url;
  if (!modem_get_menu_url(menu_url))
    return hektor_error_finding_url("menu");

  page_t menu_page;
  if (!modem_fetch_page(menu_page, menu_url))
    return hektor_error_fetching_page(menu_url);

  url_t fap_url;
  if (!modem_get_fap_url(fap_url, menu_page))
    return hektor_error_finding_url("FAP");

  page_t fap_page;
  if (!modem_fetch_page(fap_page, fap_url))
    return hektor_error_fetching_page(fap_url);

  fap_t fap;
  fap_init(&fap, fap_page);

  if (fap_is_active(&fap)) hektor_show_refill_time(&fap);
  else                     hektor_show_remaining(&fap);

  return true;
}

int main(int argc, char **argv) {
  return hektor_main(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
