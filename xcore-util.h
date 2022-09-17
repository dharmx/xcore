/* xcore-util.h
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

#include "xcore-colors.h"
#include <sys/types.h>

#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <getopt.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>

#define XCORE_VERSION "xcore-0.0.1-alpha"
#define NIL           (0)
#define WM_STATE_MASK (SubstructureRedirectMask | SubstructureNotifyMask)
#define PASS          NULL;

#define XARG_ATV(M)   (M ? stol_wrap(M) : get_active_window())

typedef struct {
    Atom _NET_WM_STATE;
    Atom _ATOM;
    Atom _NO_ATOM;
    Atom _UTF8_STRING;
    Atom _WINDOW;
    Atom _NET_WM_PID;

    Atom _NET_WM_STATE_HIDDEN;
    Atom _NET_WM_STATE_FULLSCREEN;
    Atom _NET_WM_STATE_STICKY;
    Atom _NET_DESKTOP_NAMES;
    Atom _NET_NUMBER_OF_DESKTOPS;
    Atom _WM_CLASS;
    Atom _WM_PROTOCOLS;
    Atom _CARDINAL;
    Atom _NET_ACTIVE_WINDOW;

    long _NET_WM_STATE_REMOVE;
    long _NET_WM_STATE_ADD;
    long _NET_WM_STATE_TOGGLE;
} AtomMap;

typedef struct {
    Window window;
    Window root;
    int x_offset;
    int y_offset;
    uint width;
    uint height;
    uint border;
    uint depth;
} WSize;

typedef struct {
    u_char* labels;
    u_long nlabels;
} DesktopList;

typedef struct {
    Atom* protocols;
    int count;
} WProtocols;

typedef enum {
    HIDDEN,
    UNMAPPED,
    MAPPED,
    STICKY,
    FULLSCREEN,
    CLOSE,
    KILL,
} WState;

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
void set_pointer_centered(WSize*);
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
