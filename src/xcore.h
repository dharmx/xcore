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

#include "colors.h"
#include "logger.h"
#include "tokenizer.h"
#include "util.h"

#define BEGIN_X_CALLS setup();
#define END_X_CALLS   finish();

// clang-format off
#define FETCH_TOKENS  tokenize_args(optarg, "+", ":");

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

#define FETCH_HELP_MESSAGE                                                          \
    CYN "GETTER COMMANDS:" RESET                                                    \
    BLU "\n" PADDING "active      " RESET PADDING "fetch active window id"          \
    BLU "\n" PADDING "class       " RESET PADDING "fetch window class"              \
    BLU "\n" PADDING "title       " RESET PADDING "fetch window title"              \
    BLU "\n" PADDING "geometry    " RESET PADDING "fetch window geometry"           \
    BLU "\n" PADDING "desktops    " RESET PADDING "fetch total desktops"            \
    BLU "\n" PADDING "pid         " RESET PADDING "fetch window pid"                \
    BLU "\n" PADDING "border-width" RESET PADDING "fetch window border width"       \
    BLU "\n" PADDING "keyname     " RESET PADDING "convert to keyname from keycode" \
    BLU "\n" PADDING "help        " RESET PADDING "help related to getter commands" \
    "\n\n"

#define LIST_HELP_MESSAGE                                                                     \
    CYN "LIST COMMANDS:" RESET                                                                \
    BLU "\n" PADDING "geometry   " RESET PADDING "list geometry in shell format"              \
    BLU "\n" PADDING "desktops   " RESET PADDING "list desktop names (takes a delimiter arg)" \
    BLU "\n" PADDING "protocols  " RESET PADDING "list protocols supported by window"         \
    BLU "\n" PADDING "help       " RESET PADDING "help related to list commands"              \
    "\n\n"

#define APPLY_HELP_MESSAGE                                                             \
    CYN "SETTER COMMANDS:" RESET                                                       \
    BLU "\n" PADDING "focus      " RESET PADDING "focus a window (takes a revert arg)" \
    BLU "\n" PADDING "center     " RESET PADDING "center a window"                     \
    BLU "\n" PADDING "activate   " RESET PADDING "activate a window and give it focus" \
    BLU "\n" PADDING "delay      " RESET PADDING "execute after a delay in seconds"    \
    BLU "\n" PADDING "help       " RESET PADDING "help related to setter commands"     \
    "\n\n"

#define MONITOR_HELP_MESSAGE                                                                    \
    CYN "MONITOR COMMANDS:" RESET                                                               \
    BLU "\n" PADDING "key        " RESET PADDING "print global keypresses and keyreleases"      \
    BLU "\n" PADDING "flow       " RESET PADDING "like key but allows ignore and alias args"    \
    BLU "\n" PADDING "led        " RESET PADDING "print led status"                             \
    BLU "\n" PADDING "help       " RESET PADDING "help related to monitor commands"             \
    "\n\n"

#define MONITOR_KEYBOARD_HELP_MESSAGE                                                                                    \
    CYN "KEYLOGGER COMMANDS:" RESET                                                                                      \
    BLU "\n" PADDING "config     " RESET PADDING "accepts four args for symbols first for press and second release"      \
    BLU "\n" PADDING "release    " RESET PADDING "print global keyreleases"                                              \
    BLU "\n" PADDING "press      " RESET PADDING "print global keypresses"                                               \
    BLU "\n" PADDING "help       " RESET PADDING "help related to monitor keylogger commands"                            \
    "\n\n"

#define HELP_MESSAGE                                                                                                        \
    GRN VERSION RESET                                                                                                       \
    "\n" AUTHOR "\n"                                                                                                        \
    "\n"                                                                                                                    \
    GRN "USAGE:" RESET                                                                                                      \
    "\n" PADDING "xcore [OPTIONS] <COMMAND_CHAIN> ..."                                                                      \
    "\n" PADDING "xcore [GENERAL/CHAIN_BEGIN] <CHAIN><_|CHAIN_END> ..."                                                     \
    "\n" PADDING "xcore [CHN1_BEGIN] (CHN:)[CEND_ARG|JOIN.](CHN:CARG1:CARG2...CEND_ARG) [CHN2_BEGIN] ..."                   \
    "\n\n"                                                                                                                  \
    GRN "GENERAL COMMANDS:" RESET                                                                                           \
    "\n" PADDING "-h, --help     " PADDING "print this message"                                                             \
    "\n" PADDING "-V, --version  " PADDING "print current xcore version"                                                    \
    "\n\n" FETCH_HELP_MESSAGE LIST_HELP_MESSAGE APPLY_HELP_MESSAGE MONITOR_HELP_MESSAGE MONITOR_KEYBOARD_HELP_MESSAGE

// clang-format on
void setup(void);
void finish(void);

// vim:filetype=c
