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
#include "units.h"

static void hektor_show_refill_time(const fap_t *fap) {
  enum { REFILL_TIME_LENGTH = 32 };

  const int refill_seconds = fap_refill_time_remaining(fap);
  const time_t refill_timestamp = time(NULL) + refill_seconds;

  unit_t refill_time_unit;
  unit_convert_smart(&refill_time_unit, refill_seconds, UNIT_SECOND);

  char refill_time_string[REFILL_TIME_LENGTH];
  strftime(refill_time_string, REFILL_TIME_LENGTH, "%I:%M %p %A",
           localtime(&refill_timestamp));

  printf("The FAP will be deactivated in %s, at %s.\n",
         unit_string(&refill_time_unit), refill_time_string);
}

static void hektor_show_remaining(const fap_t *fap) {
  printf("%.2f megabytes are remaining.\n",
         unit_convert(fap_usage_remaining(fap), UNIT_BYTE, UNIT_MEGABYTE));
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

  fap_t fap;
  fap_init(&fap, fap_page);

  if (fap_is_active(&fap)) hektor_show_refill_time(&fap);
  else                     hektor_show_remaining(&fap);

  return true;
}

int main(int argc, char **argv) {
  return hektor_main(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
