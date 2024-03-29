/* logger.h
 *
 * Copyright 2022 dharmx
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "util.h"

#include <X11/extensions/XInput2.h>

void init_xinput(dump_t*, int[2]);
Bool display_key(XIRawEvent*, char*, Bool);
void start_key_logger(char*, char*, Bool, RawKeyPressMode);
void start_key_flow(arg_t*, arg_t*, Bool, Bool, Bool);

void query_led_state(Bool[2]);
void start_led_logger(Bool);

// vim:filetype=c
