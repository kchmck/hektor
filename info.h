// This program is free software. It comes without any warranty, to the extent
// permitted by applicable law. You can redistribute it and/or modify it under
// the terms of the Do What The Fuck You Want To Public License, Version 2, as
// published by Sam Hocevar. See http://sam.zoy.org/wtfpl/COPYING for more
// details.

#ifndef INFO_H
#define INFO_H

#include <stdint.h>
#include <time.h>

#include "bstrlib.h"

typedef enum {
  MODEM_7000,
  MODEM_9000,

  MODEM_INVALID
} modem_t;

typedef enum {
  FAP_ACTIVE,
  FAP_INACTIVE,

  FAP_INVALID
} fap_t;

typedef struct {
  // Modem type
  modem_t modem;

  // Allowed usage in bytes
  uintmax_t usage_allowed;
  // Remaining usage in bytes
  uintmax_t usage_remain;

  // If the modem is connected to the internet
  uintmax_t conn;

  // Seconds until refilled
  uintmax_t refill_secs;
  // Timestamp when refilled
  time_t refill_ts;

  // Status of the FAP
  fap_t fap;
} info_t;

void info_init(info_t *info, const bstring page);

#endif
