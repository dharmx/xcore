/* xcore.h
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

#include "logger.h"
#include "tokenizer.h"
#include "util.h"
#include "colors.h"

#define BEGIN_X_CALLS setup();
#define END_X_CALLS   finish();

// clang-format off
#define FETCH_TOKENS  tokenize_args(optarg, "@", "::");

#define SHORT_OPTIONS "-:G:L:S:M:Vh"

#define LONG_OPTIONS                                                \
    {                                                               \
        {"fetch",     required_argument, 0, OPT_FETCH  },           \
        {"get",       required_argument, 0, OPT_FETCH  },           \
        {"list",      required_argument, 0, OPT_LIST   },           \
        {"apply",     required_argument, 0, OPT_APPLY  },           \
        {"set",       required_argument, 0, OPT_APPLY  },           \
        {"monitor",   required_argument, 0, OPT_MONITOR},           \
        {"subscribe", required_argument, 0, OPT_MONITOR},           \
        {"log",       required_argument, 0, OPT_MONITOR},           \
        {"version",   no_argument,       0, OPT_VERSION},           \
        {"help",      no_argument,       0, OPT_HELP   },           \
        {None,        None,              0, None       },           \
};

#define VERSION "xcore-0.0.3-alpha"
#define AUTHOR  MAG "dharmx" RESET " | dharmx.dev@gmail.com"
#define PADDING "     "

#define FETCH_HELP_MESSAGE                                                             \
    CYN "GETTER COMMANDS:" RESET                                                       \
    BLU "\n" PADDING "a, active      " RESET PADDING "fetch active window id"          \
    BLU "\n" PADDING "c, class       " RESET PADDING "fetch window class"              \
    BLU "\n" PADDING "t, title       " RESET PADDING "fetch window title"              \
    BLU "\n" PADDING "g, geometry    " RESET PADDING "fetch window geometry"           \
    BLU "\n" PADDING "d, desktops    " RESET PADDING "fetch total desktops"            \
    BLU "\n" PADDING "p, pid         " RESET PADDING "fetch window pid"                \
    BLU "\n" PADDING "b, border-width" RESET PADDING "fetch window border width"       \
    BLU "\n" PADDING "k, keyname     " RESET PADDING "convert to keyname from keycode" \
    BLU "\n" PADDING "h, help        " RESET PADDING "help related to getter commands" \
    "\n\n"

#define LIST_HELP_MESSAGE                                                                        \
    CYN "LIST COMMANDS:" RESET                                                                   \
    BLU "\n" PADDING "g, geometry   " RESET PADDING "list geometry in shell format"              \
    BLU "\n" PADDING "d, desktops   " RESET PADDING "list desktop names (takes a delimiter arg)" \
    BLU "\n" PADDING "p, protocols  " RESET PADDING "list protocols supported by window"         \
    BLU "\n" PADDING "h, help       " RESET PADDING "help related to list commands"              \
    "\n\n"

#define APPLY_HELP_MESSAGE                                                                \
    CYN "SETTER COMMANDS:" RESET                                                          \
    BLU "\n" PADDING "f, focus      " RESET PADDING "focus a window (takes a revert arg)" \
    BLU "\n" PADDING "c, center     " RESET PADDING "center a window"                     \
    BLU "\n" PADDING "a, activate   " RESET PADDING "activate a window and give it focus" \
    BLU "\n" PADDING "h, help       " RESET PADDING "help related to setter commands"     \
    "\n\n"

#define MONITOR_HELP_MESSAGE                                                                  \
    CYN "MONITOR COMMANDS:" RESET                                                             \
    BLU "\n" PADDING "k, keyboard   " RESET PADDING "print global keypresses and keyreleases" \
    BLU "\n" PADDING "l, led        " RESET PADDING "print led status"                        \
    BLU "\n" PADDING "h, help       " RESET PADDING "help related to monitor commands"        \
    "\n\n"

#define EXAMPLES                                                                       \
    CYN "EXAMPLES:" RESET                                                              \
    "\n" PADDING "xcore -V"                                                            \
    "\n" PADDING "xcore -V -h"                                                         \
    "\n" PADDING "xcore --version --help"                                              \
    "\n" PADDING "xcore --get active"                                                  \
    "\n" PADDING "xcore --list=protocols"                                              \
    "\n" PADDING "xcore -G active@class::0x2600006"                                    \
    "\n" PADDING "xcore -G a@t::0x3000A04geometry -L protocols::0x300002A -M keyboard" \
    "\n" PADDING "xcore -S f::0x240000A::1"                                            \
    "\n" PADDING "xcore -L desktops::\\* --list=desktops::+-"                          \
    "\n" PADDING "xcore --version --get active::"                                      \
    "\n" PADDING "xcore -S f::0x240000A::1@title::0x2000001@c"                         \
    "\n\n"

#define HELP_MESSAGE                                                                                         \
    GRN VERSION RESET                                                                                        \
    "\n" AUTHOR "\n"                                                                                         \
    "\n"                                                                                                     \
    GRN "USAGE:" RESET                                                                                       \
    "\n" PADDING "xcore [OPTIONS] <COMMAND_CHAIN> ..."                                                       \
    "\n" PADDING "xcore [GENERAL/CHAIN_BEGIN] <CHAIN><_|CHAIN_END> ..."                                      \
    "\n" PADDING "xcore [CHN1_BEGIN] (CHN::)[CEND_ARG|JOIN@](CHN::CARG1::CARG2...CEND_ARG) [CHN2_BEGIN] ..." \
    "\n\n"                                                                                                   \
    GRN "GENERAL COMMANDS:" RESET                                                                            \
    "\n" PADDING "-h, --help     " PADDING "print this message"                                              \
    "\n" PADDING "-V, --version  " PADDING "print current xcore version"                                     \
    "\n\n" FETCH_HELP_MESSAGE LIST_HELP_MESSAGE APPLY_HELP_MESSAGE MONITOR_HELP_MESSAGE EXAMPLES

// clang-format on
void setup(void);
void finish(void);

// vim:filetype=c
