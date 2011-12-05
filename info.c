// This program is free software. It comes without any warranty, to the extent
// permitted by applicable law. You can redistribute it and/or modify it under
// the terms of the Do What The Fuck You Want To Public License, Version 2, as
// published by Sam Hocevar. See http://sam.zoy.org/wtfpl/COPYING for more
// details.

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "bstrlib.h"
#include "info.h"

static bstring parse_val(const bstring page, const char *info_key) {
  const char *start = page->data;
  const char *end = page->data + page->slen;

  const char *key, *sep, *val, *nl;
  ptrdiff_t len;

  key = strstr(start, info_key);

  if (!key)
    return NULL;

  sep = memchr(key, '=', end - key);
  assert(sep);

  val = sep + 1;
  nl = memchr(val, '\n', end - val);
  assert(nl);

  len = nl - val;

  return blk2bstr(val, len);
}

static bool parse_num(uintmax_t *n, const bstring page, const char *info_key) {
  bstring val = parse_val(page, info_key);

  if (!val)
    return false;

  char *start = val->data;
  char *end;

  *n = strtoumax(start, &end, 10);

  bdestroy(val);

  return end != start;
}

static modem_t parse_modem(const bstring modem) {
  if (biseqcstr(modem, "HN7000S"))
    return MODEM_7000;

  if (biseqcstr(modem, "HN9000"))
    return MODEM_9000;

  return MODEM_INVALID;
}

static inline uintmax_t min(uintmax_t a, uintmax_t b) { return a < b ? a : b; }

void info_init(info_t *info, const bstring page) {
  bstring modem;
  uintmax_t remain[2];
  uintmax_t refill_mins;
  uintmax_t fap;

  modem = parse_val(page, "AdapterType");
  assert(modem);

  info->modem = parse_modem(modem);
  assert(info->modem != MODEM_INVALID);

  switch (info->modem) {
  case MODEM_7000:
    assert(parse_num(&info->usage_allowed, page, "FapAggBucketSize"));
  break;
  case MODEM_9000:
    assert(parse_num(&info->usage_allowed, page, "FapPbp2BucketSize"));
  break;
  }

  assert(parse_num(&remain[0], page, "FapAggBucketRemaining"));
  assert(parse_num(&remain[1], page, "FapPbp2BucketRemaining"));

  info->usage_remain = min(remain[0], remain[1]);

  assert(parse_num(&info->conn, page, "SDLPercentComplete"));

  assert(parse_num(&refill_mins, page, "TimeLeftUntilRefill"));

  info->refill_secs = refill_mins * 60;
  info->refill_ts = time(NULL) + info->refill_secs;

  assert(parse_num(&fap, page, "FapThrottleState"));

  switch (fap) {
    case 1:  info->fap = FAP_INACTIVE; break;
    case 2:  info->fap = FAP_ACTIVE; break;
    default: info->fap = FAP_INVALID; break;
  }

  assert(info->fap != FAP_INVALID);

  bdestroy(modem);
}
