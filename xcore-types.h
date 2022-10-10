/* xcore-types.h
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

#include <sys/types.h>

#include <X11/X.h>
#include <X11/Xlib.h>

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
    u_int width;
    u_int height;
    u_int border;
    u_int depth;
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

typedef struct {
    int major_opcode_return;
    int first_event_return;
    int first_error_return;
    Bool query_result;

    int opcode_rtrn;
    int event_rtrn;
    int error_rtrn;
    int major_in_out;
    int minor_in_out;
    int xkb_query_result;

    Status xi_query_result;
} dump_t;

typedef enum {
    ONLY_KEY_UP,
    ONLY_KEY_DOWN,
    BOTH_KEY_UP_DOWN,
} RawKeyPressMode;

typedef struct arg_t arg_t;
struct arg_t {
    char* value;
    arg_t* next_arg;
};

typedef struct token_t token_t;
struct token_t {
    char* option;
    arg_t* args;
    u_long nargs;
    token_t* next_token;
};

// vim:filetype=c
