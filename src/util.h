/* util.h
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

#include "types.h"
#include <sys/types.h>

#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XInput2.h>

#include <getopt.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NIL                (0)
#define WM_STATE_MASK      (SubstructureRedirectMask | SubstructureNotifyMask)
#define PASS               NULL;

#define XARG_ATV(M)        (M ? stol_wrap(M) : get_active_window())
#define XARG_NULL(M, N, S) (S <= M ? stol_wrap(N) : get_active_window())

extern Display* display;

// util functions
void log_error(int, const char*);
long stol_wrap(const char*);

// display functions
void display_window_geometry(Window, Bool);
void display_desktops(char*);
void display_window_class(Window);
void display_window_protocols(Window);
void display_window_title(Window);

// setter functions
void set_pointer_centered(Drawable);
int set_window_state(Window, WState, Bool);
void set_window_focus(Window, int);
// @see https://specifications.freedesktop.org/wm-spec/wm-spec-latest.html#idm46409506372544
void set_window_activate(Window);

// getter functions
Window get_active_window();
WSize* get_window_geometry(Drawable);
DesktopList* get_desktops();
long get_total_desktops();
long get_window_pid(Window);
WProtocols* get_window_protocols(Window);
XTextProperty* get_window_title(Window);
u_long get_window_border(Window);
char* get_keyname_from_code(KeyCode);

// init functions
void init_atoms(AtomMap*, Bool);

// vim:filetype=c
