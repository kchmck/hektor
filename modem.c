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
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <curl/curl.h>

#include "common.h"
#include "modem.h"

void modem_global_init(void) {
  curl_global_init(CURL_GLOBAL_NOTHING);
}

void modem_global_destroy(void) {
  curl_global_cleanup();
}

// Request a url with a callback function.
static bool modem_fetch(const url_t url, void *receive_fn, void *fn_data) {
  CURL *curl = curl_easy_init();
  if (!curl) return false;

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, receive_fn);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, fn_data);

  const CURLcode result = curl_easy_perform(curl);
  curl_easy_cleanup(curl);

  return result == CURLE_OK;
}

// Discard any downloaded data.
static size_t discard_fn(const char *chunk, const size_t item_size,
                         const size_t items, void *state)
{
  return items * item_size;
}

// Simply fetch a @url and discard any downloaded data.
static bool modem_fetch_simple(const url_t url) {
  return modem_fetch(url, discard_fn, NULL);
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
  const size_t buffer_remaining = PAGE_MAX_LENGTH - state->amount_written;

  if (buffer_remaining <= 0) return 0;

  const size_t write_size = min(chunk_size, buffer_remaining);

  memcpy(&state->buffer[state->amount_written], chunk, write_size);
  state->amount_written += write_size;

  return write_size;
}

// Strip the leading slash off a path and append the rest to the modem's base
// url.
static inline bool modem_build_url(url_t url_buffer, const url_t url) {
  return snprintf(url_buffer, URL_MAX_LENGTH, "http://192.168.0.1/%s", &url[1]);
}

bool modem_get_info_url(url_t buffer) {
  return modem_build_url(buffer, "/getdeviceinfo/info.bin");
}

bool modem_get_restart_url(url_t buffer) {
  return modem_build_url(buffer, "/com/gatewayreset/");
}

size_t modem_fetch_page(page_t buffer, const url_t url) {
  download_fn_state_t state = {buffer};
  modem_fetch(url, download_fn, &state);

  buffer[state.amount_written] = '\0';

  return state.amount_written;
}

bool modem_restart(const url_t restart_url) {
  return modem_fetch_simple(restart_url);
}
