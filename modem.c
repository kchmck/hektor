// This program is free software. It comes without any warranty, to the extent
// permitted by applicable law. You can redistribute it and/or modify it under
// the terms of the Do What The Fuck You Want To Public License, Version 2, as
// published by Sam Hocevar. See http://sam.zoy.org/wtfpl/COPYING for more
// details.

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#include <curl/curl.h>

#include "bstrlib.h"
#include "modem.h"

static size_t discard_fn(void *chunk, size_t size, size_t num, void *data) {
  return size * num;
}

bool modem_touch(const char *url) {
  CURL *curl;
  CURLcode ret;

  curl = curl_easy_init();
  assert(curl);

  curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, discard_fn);

  ret = curl_easy_perform(curl);

  curl_easy_cleanup(curl);

  return ret == CURLE_OK;
}

static size_t write_fn(void *chunk, size_t size, size_t num, void *data) {
  bstring buf = data;
  size_t len = size * num;

  assert(bcatblk(buf, chunk, len) == BSTR_OK);

  return len;
}

bstring modem_fetch(const char *url) {
  bstring buf;
  CURL *curl;
  CURLcode ret;

  buf = bfromcstralloc(512, "");
  assert(buf);

  curl = curl_easy_init();
  assert(curl);

  curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, buf);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_fn);

  ret = curl_easy_perform(curl);

  curl_easy_cleanup(curl);

  if (ret == CURLE_OK)
    return buf;

  bdestroy(buf);

  return NULL;
}
