// This program is free software. It comes without any warranty, to the extent
// permitted by applicable law. You can redistribute it and/or modify it under
// the terms of the Do What The Fuck You Want To Public License, Version 2, as
// published by Sam Hocevar. See http://sam.zoy.org/wtfpl/COPYING for more
// details.

#ifndef MODEM_H
#define MODEM_H

#include <stdbool.h>
#include "bstrlib.h"

static const char *MODEM_INFO_URL =
  "http://192.168.0.1/getdeviceinfo/info.bin";

static const char *MODEM_RESTART_URL =
  "http://192.168.0.1/com/gatewayreset/";

bool modem_touch(const char *url);
bstring modem_fetch(const char *url);

#endif
