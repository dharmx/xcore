/* logger.c
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

#include "logger.h"

#include <X11/XKBlib.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ON_OR_OFF(S) (S ? "on" : "off")

void
init_xinput(dump_t* dump, int version[2]) {
    // clang-format off
    dump->query_result     = XQueryExtension(display, "XInputExtension", &dump->major_opcode_return, &dump->first_event_return, &dump->first_error_return);
    dump->xi_query_result  = XIQueryVersion(display, &version[0], &version[1]);
    dump->xkb_query_result = XkbQueryExtension(display, &dump->opcode_rtrn, &dump->event_rtrn, &dump->error_rtrn, &dump->major_in_out, &dump->minor_in_out);
    // clang-format on
}

Bool
display_key(XIRawEvent* event, char* prefix, Bool only_icon) {
    KeySym keysym = XkbKeycodeToKeysym(display, event->detail, None, None);
    if (keysym == NoSymbol) return False;
    char* keyname = XKeysymToString(keysym);
    if (keyname == NULL) return False;

    printf("%s%s\n", prefix, only_icon ? "" : keyname);
    fflush(stdout);
    return True;
}

void
start_key_flow(arg_t* aliases, arg_t* ignored, Bool led_state, Bool apply_mod, Bool apply_hold) {
    dump_t dump;
    int version[2] = {2, 0};
    memset(&dump, None, sizeof(dump_t));
    init_xinput(&dump, version);

    if (!dump.query_result) perror("ERROR: XInputExtension does not exist!");
    if (dump.xi_query_result != Success) perror("ERROR: XI2 is not supported!");
    if (!dump.xkb_query_result) perror("ERROR: XKBlib is not compatible!");

    XIEventMask mask;
    memset(&mask, None, sizeof(XIEventMask));
    mask.mask_len = XIMaskLen(XI_LASTEVENT);
    mask.deviceid = XIAllMasterDevices;
    mask.mask     = calloc(1, sizeof(u_char));
    XISetMask(mask.mask, XI_RawKeyPress);
    XISetMask(mask.mask, XI_RawKeyRelease);

    XISelectEvents(display, DefaultRootWindow(display), &mask, 1);
    XSync(display, False);
    XFree(mask.mask);
    XkbSelectEventDetails(display, XkbUseCoreKbd, XkbStateNotify, XkbGroupStateMask, XkbGroupStateMask);
    XkbStateRec state;
    XkbGetState(display, XkbUseCoreKbd, &state);
    int group = state.group;

    XEvent event;
    memset(&event, None, sizeof(XEvent));
    XGenericEventCookie* cookie;

    Bool led_states[2] = {False, False};
    Bool shift_pressed = False;
    query_led_state(led_states);
    while (True) {
        cookie = &event.xcookie;
        XNextEvent(display, &event);

        // clang-format off
        if (XGetEventData(display, cookie) && cookie->type == GenericEvent && cookie->extension == dump.major_opcode_return) {
            // clang-format on
            XIRawEvent* raw_event = cookie->data;

            if (cookie->evtype == XI_RawKeyPress) {
                KeySym keysym = XkbKeycodeToKeysym(display, raw_event->detail, group, None);
                if (keysym == NoSymbol) continue;
                char* keyname = XKeysymToString(keysym);

                arg_t* arg_ignore = ignored;
                Bool skip         = False;
                while (arg_ignore != NULL) {
                    if (!strcmp(arg_ignore->value, keyname)) skip = True;
                    arg_ignore = arg_ignore->next_arg;
                }

                arg_t* arg_alias = aliases;
                while (arg_alias != NULL && arg_alias->next_arg != NULL) {
                    if (!strcmp(arg_alias->value, keyname)) keyname = arg_alias->next_arg->value;
                    arg_alias = arg_alias->next_arg->next_arg;
                }

                if (apply_mod) {
                    switch (raw_event->detail) {
                        case 38:
                        case 56:
                        case 54:
                        case 40:
                        case 26:
                        case 41:
                        case 42:
                        case 43:
                        case 31:
                        case 44:
                        case 45:
                        case 46:
                        case 58:
                        case 57:
                        case 32:
                        case 33:
                        case 24:
                        case 27:
                        case 39:
                        case 28:
                        case 30:
                        case 55:
                        case 25:
                        case 53:
                        case 29:
                        case 52: {
                            char first = keyname[0];
                            if (led_state && led_states[0]) first = toupper(first);
                            if (shift_pressed) first = isupper(first) ? tolower(first) : toupper(first);
                            char _keyname[2] = {first, '\0'};
                            keyname          = _keyname;
                            break;
                        }
                        case 50:
                        case 62: shift_pressed = True; break;
                    }
                }

                Bool printed = False;
                if (led_state) {
                    switch (raw_event->detail) {
                        case 66:
                            led_states[0] = !led_states[0];
                            printf("%s %s\n", keyname, led_states[0] ? "on" : "off");
                            printed = True;
                            break;
                        case 77:
                            led_states[1] = !led_states[1];
                            printf("%s %s\n", keyname, led_states[1] ? "on" : "off");
                            printed = True;
                            break;
                    }
                }

                if (skip == True) continue;
                if (!printed) printf("%s\n", keyname);

                fflush(stdout);
            } else if (cookie->evtype == XI_RawKeyRelease) {
                if (apply_mod) {
                    switch (raw_event->detail) {
                        case 50:
                        case 62: shift_pressed = False; break;
                    }
                }
            }
            XFreeEventData(display, cookie);
        }
    }
}

void
start_key_logger(char* release, char* press, Bool only_icon, RawKeyPressMode mode) {
    if (release == NULL) release = "KeyRelease ";
    if (press == NULL) press = "KeyPress ";

    dump_t dump;
    int version[2] = {2, 0};
    memset(&dump, None, sizeof(dump_t));
    init_xinput(&dump, version);

    if (!dump.query_result) perror("ERROR: XInputExtension does not exist!");
    if (dump.xi_query_result != Success) perror("ERROR: XI2 is not supported!");
    if (!dump.xkb_query_result) perror("ERROR: XKBlib is not compatible!");

    XIEventMask mask;
    memset(&mask, None, sizeof(XIEventMask));
    mask.mask_len = XIMaskLen(XI_LASTEVENT);
    mask.deviceid = XIAllMasterDevices;
    mask.mask     = calloc(1, sizeof(u_char));

    switch (mode) {
        case ONLY_KEY_DOWN: XISetMask(mask.mask, XI_RawKeyPress); break;
        case ONLY_KEY_UP: XISetMask(mask.mask, XI_RawKeyRelease); break;
        case BOTH_KEY_UP_DOWN:
            XISetMask(mask.mask, XI_RawKeyPress);
            XISetMask(mask.mask, XI_RawKeyRelease);
            break;
        default: perror("undefined mode"); break;
    }

    XISelectEvents(display, DefaultRootWindow(display), &mask, 1);
    XFree(mask.mask);

    XEvent event;
    memset(&event, None, sizeof(XEvent));
    XGenericEventCookie* cookie;

    while (True) {
        cookie = &event.xcookie;
        XNextEvent(display, &event);

        // clang-format off
        if (XGetEventData(display, cookie) && cookie->type == GenericEvent && cookie->extension == dump.major_opcode_return) {
            XIRawEvent* raw_event = cookie->data;

            switch (cookie->evtype) {
                case XI_RawKeyPress:
                    if (!display_key(raw_event, press, only_icon)) continue;
                    break;
                case XI_RawKeyRelease:
                    if (!display_key(raw_event, release, only_icon)) continue;
                    break;
                default: continue;
            }
            XFreeEventData(display, cookie);
        }
        // clang-format on
    }
}

void
query_led_state(Bool ind_keys[2]) {
    unsigned int start_state;
    XkbGetIndicatorState(display, XkbUseCoreKbd, &start_state);
    if (ind_keys == NULL) {
        ind_keys[0] = False;
        ind_keys[1] = False;
    }
    switch (start_state) {
        case NO_STATE: ind_keys[0] = ind_keys[1] = False; break;
        case CAPS_LOCK_STATE: ind_keys[0] = True; break;
        case NUM_LOCK_STATE: ind_keys[1] = True; break;
        case BOTH_LOCK_STATE: ind_keys[0] = ind_keys[1] = True; break;
    }
}

void
start_led_logger(Bool first) {
    Bool ind_keys[2] = {False, False};
    if (first) {
        query_led_state(ind_keys);
        printf("indicators %s %s\n", ON_OR_OFF(ind_keys[0]), ON_OR_OFF(ind_keys[1]));
        fflush(stdout);
    }

    XkbSelectEvents(display, XkbUseCoreKbd, XkbIndicatorStateNotifyMask, XkbIndicatorStateNotifyMask);
    XkbIndicatorNotifyEvent event;
    while (True) {
        XNextEvent(display, (XEvent*) &event);
        if (event.xkb_type == XkbIndicatorStateNotify) {
            query_led_state(ind_keys);
            printf("indicators %s %s\n", ON_OR_OFF(ind_keys[0]), ON_OR_OFF(ind_keys[1]));
            fflush(stdout);
        }
    }
}

// vim:filetype=c
