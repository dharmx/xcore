/* xcore.c
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

#include "xcore.h"

Display* display;

int
main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "ERROR: No arguments given.\n");
        exit(EXIT_FAILURE);
    }

    BEGIN_X_CALLS;

    int option_item;
    int option_index;
    const char* short_options = SHORT_OPTIONS;
    enum { OPT_FETCH = 'G', OPT_LIST = 'L', OPT_APPLY = 'S', OPT_MONITOR = 'M', OPT_VERSION = 'V', OPT_HELP = 'h' };
    const struct option long_options[] = LONG_OPTIONS;

    // clang-format off
    token_t* tokens = NULL;
    while ((option_item = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1) {
        switch (option_item) {
            case OPT_FETCH:
                tokens = FETCH_TOKENS
                while (tokens != NULL) {
                    if (strlen(tokens->option) == 1) {
                        switch (tokens->option[0]) {
                            case 'a': printf("0x%lX\n", get_active_window()); break;
                            case 'c': display_window_class(XARG_NULL(tokens->nargs, tokens->args->value, 1)); break;
                            case 't': display_window_title(XARG_NULL(tokens->nargs, tokens->args->value, 1)); break;
                            case 'g': display_window_geometry(XARG_NULL(tokens->nargs, tokens->args->value, 1), False); break;
                            case 'd': printf("%ld\n", get_total_desktops()); break;
                            case 'p': printf("%ld\n", get_window_pid(XARG_NULL(tokens->nargs, tokens->args->value, 1))); break;
                            case 'b': printf("%ld\n", get_window_border(XARG_NULL(tokens->nargs, tokens->args->value, 1))); break;
                            case 'k': printf("%s\n", get_keyname_from_code(XARG_NULL(tokens->nargs, tokens->args->value, 1))); break;
                            case 'h': printf(FETCH_HELP_MESSAGE); break;
                            default: printf("ERROR: undefined option '%s'\n", tokens->option); exit(EXIT_FAILURE); break;
                        }
                    } else {
                        if (!strcmp(tokens->option, "active")) printf("0x%lX\n", get_active_window());
                        else if (!strcmp(tokens->option, "class")) display_window_class(XARG_NULL(tokens->nargs, tokens->args->value, 1));
                        else if (!strcmp(tokens->option, "title")) display_window_title(XARG_NULL(tokens->nargs, tokens->args->value, 1));
                        else if (!strcmp(tokens->option, "geometry")) display_window_geometry(XARG_NULL(tokens->nargs, tokens->args->value, 1), False);
                        else if (!strcmp(tokens->option, "desktops")) printf("%ld\n", get_total_desktops());
                        else if (!strcmp(tokens->option, "pid")) printf("%ld\n", get_window_pid(XARG_NULL(tokens->nargs, tokens->args->value, 1)));
                        else if (!strcmp(tokens->option, "border-width")) printf("%ld\n", get_window_border(XARG_NULL(tokens->nargs, tokens->args->value, 1)));
                        else if (!strcmp(tokens->option, "keyname")) printf("%s\n", get_keyname_from_code(XARG_NULL(tokens->nargs, tokens->args->value, 1)));
                        else if (!strcmp(tokens->option, "help") || !strcmp(tokens->option, "-h") || !strcmp(tokens->option, "--help")) printf(FETCH_HELP_MESSAGE);
                        else { printf("ERROR: undefined option '%s'\n", tokens->option); exit(EXIT_FAILURE); }
                    }
                    tokens = tokens->next_token;
                }
                break;
            case OPT_LIST:
                tokens = FETCH_TOKENS
                while (tokens != NULL) {
                    if (strlen(tokens->option) == 1) {
                        switch (tokens->option[0]) {
                            case 'g': display_window_geometry(XARG_NULL(tokens->nargs, tokens->args->value, 1), True); break;
                            case 'd': display_desktops(tokens->nargs == 1 ? tokens->args->value : NULL); break;
                            case 'p': display_window_protocols(XARG_NULL(tokens->nargs, tokens->args->value, 1)); break;
                            case 'h': printf(LIST_HELP_MESSAGE); break;
                            default: printf("ERROR: undefined option '%s'\n", tokens->option); exit(EXIT_FAILURE); break;
                        }
                    } else {
                        if (!strcmp(tokens->option, "geometry")) display_window_geometry(XARG_NULL(tokens->nargs, tokens->args->value, 1), True);
                        else if (!strcmp(tokens->option, "desktops")) display_desktops(tokens->nargs == 1 ? tokens->args->value : NULL);
                        else if (!strcmp(tokens->option, "protocols")) display_window_protocols(XARG_NULL(tokens->nargs, tokens->args->value, 1));
                        else if (!strcmp(tokens->option, "help") || !strcmp(tokens->option, "-h") || !strcmp(tokens->option, "--help")) printf(LIST_HELP_MESSAGE);
                        else { printf("ERROR: undefined option '%s'\n", tokens->option); exit(EXIT_FAILURE); }
                    }
                    tokens = tokens->next_token;
                }
                break;
            case OPT_APPLY:
                tokens = FETCH_TOKENS
                while (tokens != NULL) {
                    if (strlen(tokens->option) == 1) {
                        switch (tokens->option[0]) {
                            // WARN: write get_arg(int index) utility instead of using XARG_NULL
                            case 'f': set_window_focus(XARG_NULL(tokens->nargs, tokens->args->value, 1), XARG_NULL(tokens->nargs, tokens->args->next_arg->value, 2)); break;
                            case 'c': set_pointer_centered(XARG_NULL(tokens->nargs, tokens->args->value, 1)); break;
                            case 'a': set_window_activate(XARG_NULL(tokens->nargs, tokens->args->value, 1)); break;
                            case 'h': printf(APPLY_HELP_MESSAGE); break;
                            default: printf("ERROR: undefined option '%s'\n", tokens->option); exit(EXIT_FAILURE); break;
                        }
                    } else {
                        if (!strcmp(tokens->option, "focus")) set_window_focus(XARG_NULL(tokens->nargs, tokens->args->value, 1), False);
                        else if (!strcmp(tokens->option, "center")) set_pointer_centered(XARG_NULL(tokens->nargs, tokens->args->value, 1));
                        else if (!strcmp(tokens->option, "activate")) set_window_activate(XARG_NULL(tokens->nargs, tokens->args->value, 1));
                        else if (!strcmp(tokens->option, "help") || !strcmp(tokens->option, "-h") || !strcmp(tokens->option, "--help")) printf(APPLY_HELP_MESSAGE);
                        else { printf("ERROR: undefined option '%s'\n", tokens->option); exit(EXIT_FAILURE); }
                    }
                    tokens = tokens->next_token;
                }
                break;
            case OPT_MONITOR:
                tokens = FETCH_TOKENS
                while (tokens != NULL) {
                    if (strlen(tokens->option) == 1) {
                        switch (tokens->option[0]) {
                            case 'k': start_key_logger(NULL, NULL, False, BOTH_KEY_UP_DOWN); break;
                            case 'l': start_led_logger(XARG_NULL(tokens->nargs, tokens->args->next_arg->value, 1) ? False : True); break;
                            case 'h': printf(MONITOR_HELP_MESSAGE); break;
                            default: printf("ERROR: undefined option '%s'\n", tokens->option); exit(EXIT_FAILURE); break;
                        }
                    } else {
                        if (!strcmp(tokens->option, "keyboard")) start_key_logger(NULL, NULL, False, BOTH_KEY_UP_DOWN);
                        else if (!strcmp(tokens->option, "led")) start_led_logger(XARG_NULL(tokens->nargs, tokens->args->next_arg->value, 1) ? False : True);
                        else if (!strcmp(tokens->option, "help") || !strcmp(tokens->option, "-h") || !strcmp(tokens->option, "--help")) printf(MONITOR_HELP_MESSAGE);
                        else { printf("ERROR: undefined option '%s'\n", tokens->option); exit(EXIT_FAILURE); }
                    }
                    tokens = tokens->next_token;
                }
                break;
            case OPT_VERSION: printf(VERSION); break;
            case OPT_HELP: printf(HELP_MESSAGE); break;
            case '?': fprintf(stderr, "ERROR: invalid general argument\n"); printf(HELP_MESSAGE); break;
            default: printf("ERROR: undefined action\n"); exit(EXIT_FAILURE); break;
        }
    }
    // clang-format off

    END_X_CALLS;
    return EXIT_SUCCESS;
}

void
setup(void) {
    display = XOpenDisplay(None);
}

void
finish(void) {
    XSync(display, None);
    XFlush(display);
    XCloseDisplay(display);
}

// vim:filetype=c
