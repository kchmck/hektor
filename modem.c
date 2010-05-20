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
#include <string.h>

#include <curl/curl.h>

#include "common.h"
#include "modem.h"

// Append a path to the modem's base url.
static inline int build_url(url_t url_buffer, const url_t url) {
  // Strip off the leading '/'.
  return snprintf(url_buffer, MAX_URL_LENGTH, "http://192.168.0.1/%s", &url[1]);
}

// Request a url with a callback function.
static bool modem_fetch_url(const url_t url, void *receive_fn, void *fn_data) {
  url_t full_url;
  if (!build_url(full_url, url)) return false;

  CURL *curl = curl_easy_init();
  if (!curl) return false;

  curl_easy_setopt(curl, CURLOPT_URL, full_url);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, receive_fn);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, fn_data);

  CURLcode result = curl_easy_perform(curl);
  curl_easy_cleanup(curl);

  return result == CURLE_OK;
}

// Search through the menu page for a page title and its associated url. The
// menu page is laid out like:
//
//          ┌──────┬─ page_title
//   [...] "FAP Info", "/cgi/execAdvCom.bin?Command=123&PrintMsg=FAP Info"))
//                      └─┬─────────────────────────────────────────────┘
//                        └─ url copied into url_buffer
//
// The variables below end up like:
//
//          ┌─ title_begin
//          │           ┌─ url_begin                            url_end ─┐
//   [...] "FAP Info", "/cgi/execAdvCom.bin?Command=123&PrintMsg=FAP Info"))
//          url_length ─┴───────────────────────────────────────────────┘
//
static bool menu_find_url(url_t url_buffer, const page_t menu_page,
                          const char *page_title)
{
  static const char SEPARATOR[] = "\", \"";
  enum { SEPARATOR_LENGTH = sizeof(SEPARATOR) - 1 };

  static const char URL_ENDING[] = "\"))";

  // Try to match the page title.
  const char *title_begin = strstr(menu_page, page_title);
  if (!title_begin) return false;

  const char *url_begin = title_begin
                        + strlen(page_title)
                        + SEPARATOR_LENGTH;

  const char *url_end = strstr(url_begin, URL_ENDING);
  if (!url_end) return false;

  const int url_length = url_end - url_begin;

  // Add 1 for null termination.
  string_copy(url_begin, url_buffer, min(url_length + 1, MAX_URL_LENGTH));

  return true;
}

bool modem_get_menu_url(url_t buffer) {
  return string_copy("/fs/scripts/cat_menu.js", buffer, MAX_URL_LENGTH);
}

bool modem_get_fap_url(url_t buffer, const page_t menu_page) {
  return menu_find_url(buffer, menu_page, "FAP Info");
}

// Copy chunks of a page to a buffer...
typedef struct {
  char *buffer;
  size_t amount_written;
} download_fn_state_t;

static size_t download_fn(const char *chunk, const size_t item_size,
                          const size_t items, download_fn_state_t *state)
{
  const size_t chunk_size = item_size * items;
  const size_t buffer_remaining = MAX_PAGE_SIZE - state->amount_written;

  if (buffer_remaining <= 0) return 0;

  const size_t write_size = min(chunk_size, buffer_remaining);

  memcpy(&state->buffer[state->amount_written], chunk, write_size);
  state->amount_written += write_size;

  return chunk_size;
}

size_t modem_fetch_page(page_t buffer, const url_t url) {
  download_fn_state_t state = {buffer};
  modem_fetch_url(url, download_fn, &state);

  return state.amount_written;
}
