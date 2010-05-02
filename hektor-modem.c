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

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <curl/curl.h>

#include "hektor-modem.h"

// Append a path to the modem's base url.
static inline int modem_build_url(const url_t url, url_t url_buffer) {
  // Strip off the leading '/'.
  return snprintf(url_buffer, MAX_URL_LENGTH, "http://192.168.0.1/%s", &url[1]);
}

// Request a url with a callback function.
static CURLcode modem_fetch_url(const url_t url, void *const receive_fn,
                                void *const fn_data) 
{
  url_t full_url;
  modem_build_url(url, full_url);

  CURL *const curl = curl_easy_init();

  curl_easy_setopt(curl, CURLOPT_URL, full_url);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, receive_fn);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, fn_data);

  CURLcode result = curl_easy_perform(curl);
  curl_easy_cleanup(curl);

  return result;
}

bool modem_find_url(const char *const page_title, const page_t menu_page,
                    url_t url_buffer)
{
  // The little bit of text between a page title and its url
  #define TITLE_URL_SEP "\", \""
  // The little bit of text at the end of a url
  #define URL_ENDING    "\"))"

  // Try to match the page title.
  const char *const title_match = strstr(menu_page, page_title);
  if (!title_match) return false;

  const size_t title_begin = title_match - menu_page;
  const size_t url_begin = title_begin
                         + strlen(page_title)
                         + strlen(TITLE_URL_SEP);

  const char *const url_end_match = strstr(&menu_page[url_begin], URL_ENDING);
  if (!url_end_match) return false;

  const size_t url_end = url_end_match - menu_page;
  const size_t url_length = url_end - url_begin;

  strncpy(url_buffer, &menu_page[url_begin], min(url_length, MAX_PATH_LENGTH));

  #undef TITLE_URL_SEP
  #undef URL_ENDING

  return true;
}

// Used to keep state between chunks.
typedef struct {
  char *const buffer;
  size_t amount_written;
} buffer_append_state_t;

// Append successive chunks of a page to a buffer.
static size_t buffer_append_fn(const char *const chunk,
                               const size_t item_size,
                               const size_t items,
                               buffer_append_state_t *const state)
{
  const size_t chunk_size = item_size * items;
  const size_t buffer_remaining = MAX_PAGE_SIZE - state->amount_written;

  if (buffer_remaining <= 0) return 0;

  memcpy(&state->buffer[state->amount_written], chunk, min(chunk_size,
                                                           buffer_remaining));
  state->amount_written += chunk_size;

  return chunk_size;
}

size_t modem_fetch_page(const url_t url, page_t buffer) {
  memset(buffer, 0, sizeof(page_t));

  buffer_append_state_t state = {buffer};
  modem_fetch_url(url, buffer_append_fn, &state);

  return state.amount_written;
}
