#ifndef HEKTOR_ERROR_H
#define HEKTOR_ERROR_H

#include <stdbool.h>

#include "hektor-config.h"
#include "hektor-snapshots.h"

bool hektor_error_loading_config(config_t *const config);

bool hektor_error_loading_plan(config_t *const config);

bool hektor_error_invalid_plan(const config_string_t plan_name);

bool hektor_error_loading_snapshots(const snapshots_t *const snapshots);

bool hektor_error_saving_snapshots(const snapshots_t *const snapshots);

bool hektor_error_invalid_command(const char *const user_command);

bool hektor_error_with_modem(void);

#endif
