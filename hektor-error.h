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

#ifndef HEKTOR_ERROR_H
#define HEKTOR_ERROR_H

#include <stdbool.h>

#include "hektor-config.h"
#include "hektor-snapshots.h"

bool hektor_error_loading_config(config_t *config);

bool hektor_error_loading_plan(config_t *config);

bool hektor_error_invalid_plan(const config_string_t plan_name);

bool hektor_error_loading_snapshots(const snapshots_t *snapshots);

bool hektor_error_saving_snapshots(const snapshots_t *snapshots);

bool hektor_error_invalid_command(const char *user_command);

bool hektor_error_with_modem(void);

#endif
