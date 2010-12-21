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
static size_t discard_fn(void *chunk, size_t size, size_t num, void *data) {
  return size * num;
}

// Simply fetch a @url and discard any downloaded data.
static bool modem_fetch_simple(const url_t url) {
  return modem_fetch(url, discard_fn, NULL);
}

// Copy chunks of a page to a buffer...
typedef struct {
  char *buffer;
  size_t written;
} response_t;

void response_init(response_t *response, char buffer[]) {
  response->buffer = buffer;
  response->written = 0;
}

size_t response_finish(response_t *response) {
  response->buffer[response->written] = '\0';
  return response->written;
}

static size_t request_fn(void *chunk, size_t size, size_t num, void *data) {
  response_t *response = data;

  const size_t chunk_size = size * num;
  const size_t buffer_remaining = max(PAGE_LENGTH - response->written, 0);

  if (!buffer_remaining) return 0;

  const size_t write_size = min(chunk_size, buffer_remaining);

  memcpy(&response->buffer[response->written], chunk, write_size);
  response->written += write_size;

  return write_size;
}

// Strip the leading slash off a path and append the rest to the modem's base
// url.
static inline bool modem_build_url(url_t url_buffer, const url_t url) {
  return snprintf(url_buffer, URL_LENGTH, "http://192.168.0.1/%s", &url[1]);
}

bool modem_build_info_url(url_t buffer) {
  return modem_build_url(buffer, "/getdeviceinfo/info.bin");
}

bool modem_build_restart_url(url_t buffer) {
  return modem_build_url(buffer, "/com/gatewayreset/");
}

size_t modem_fetch_page(page_t buffer, const url_t url) {
  response_t response;
  response_init(&response, buffer);

  modem_fetch(url, request_fn, &response);

  return response_finish(&response);
}

bool modem_restart(const url_t restart_url) {
  return modem_fetch_simple(restart_url);
}
