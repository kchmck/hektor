// This program is free software. It comes without any warranty, to the extent
// permitted by applicable law. You can redistribute it and/or modify it under
// the terms of the Do What The Fuck You Want To Public License, Version 2, as
// published by Sam Hocevar. See http://sam.zoy.org/wtfpl/COPYING for more
// details.

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>
#include <yajl/yajl_gen.h>

#include "info.h"
#include "modem.h"

int main(int argc, char **argv) {
  bstring info_page;
  info_t info;
  yajl_gen yajl;
  const unsigned char *json;
  size_t json_len;

  curl_global_init(CURL_GLOBAL_NOTHING);

  for (size_t i = 0; argv[i]; i += 1) {
    if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--restart") == 0) {
      modem_touch(MODEM_RESTART_URL);
      return EXIT_SUCCESS;
    }
  }

  info_page = modem_fetch(MODEM_INFO_URL);

  if (!info_page) {
    fprintf(stderr, "hektor: Couldn't fetch '%s'\n", MODEM_INFO_URL);
    return EXIT_FAILURE;
  }

  info_init(&info, info_page);

  yajl = yajl_gen_alloc(NULL);
  assert(yajl);

  yajl_gen_config(yajl, yajl_gen_beautify, true);

#define yajl_gen_strst(y, s) yajl_gen_string((y), (s), sizeof(s) - 1)

  yajl_gen_map_open(yajl);
    yajl_gen_strst(yajl, "connection");
    yajl_gen_integer(yajl, info.conn);

    yajl_gen_strst(yajl, "fapped");
    yajl_gen_bool(yajl, info.fap == FAP_ACTIVE);

    yajl_gen_strst(yajl, "refill_secs");
    yajl_gen_integer(yajl, info.refill_secs);

    yajl_gen_strst(yajl, "refill_ts");
    yajl_gen_integer(yajl, info.refill_ts);

    yajl_gen_strst(yajl, "usage_allowed");
    yajl_gen_integer(yajl, info.usage_allowed);

    yajl_gen_strst(yajl, "usage_remain");
    yajl_gen_integer(yajl, info.usage_remain);
  yajl_gen_map_close(yajl);

#undef yajl_gen_strst

  yajl_gen_get_buf(yajl, &json, &json_len);
  printf("%s", json);

  bdestroy(info_page);
  yajl_gen_free(yajl);
  curl_global_cleanup();

  return EXIT_SUCCESS;
}
