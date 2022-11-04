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
#define ON_OR_OFF(S) (S ? "on" : "off")

void
init_xinput(dump_t* dump, int version[2]) {
    dump->query_result     = XQueryExtension(display,
                                         "XInputExtension",
                                         &dump->major_opcode_return,
                                         &dump->first_event_return,
                                         &dump->first_error_return);
    dump->xi_query_result  = XIQueryVersion(display, &version[0], &version[1]);
    dump->xkb_query_result = XkbQueryExtension(display,
                                               &dump->opcode_rtrn,
                                               &dump->event_rtrn,
                                               &dump->error_rtrn,
                                               &dump->major_in_out,
                                               &dump->minor_in_out);
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
start_key_logger(char* up, char* down, Bool only_icon, RawKeyPressMode mode) {
    if (up == NULL) up = "~";
    if (down == NULL) down = "@";

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

        if (XGetEventData(display, cookie) && cookie->type == GenericEvent &&
            cookie->extension == dump.major_opcode_return) {
            XIRawEvent* raw_event = cookie->data;

            switch (cookie->evtype) {
                case XI_RawKeyPress:
                    if (!display_key(raw_event, down, only_icon)) continue;
                    break;
                case XI_RawKeyRelease:
                    if (!display_key(raw_event, up, only_icon)) continue;
                    break;
                default: continue;
            }
            XFreeEventData(display, cookie);
        }
    }
}

void
display_ind_state(Bool ind_keys[2], int event_state) {
    typedef enum {
        NO_STATE        = 0,
        CAPS_LOCK_STATE = 1,
        NUM_LOCK_STATE  = 2,
        BOTH_LOCK_STATE = 3,
    } ind_state_t;

    switch (event_state) {
        case NO_STATE: ind_keys[0] = ind_keys[1] = False; break;
        case CAPS_LOCK_STATE: ind_keys[0] = True; break;
        case NUM_LOCK_STATE: ind_keys[1] = True; break;
        case BOTH_LOCK_STATE: ind_keys[0] = ind_keys[1] = True; break;
    }

    printf("indicators %s %s\n", ON_OR_OFF(ind_keys[0]), ON_OR_OFF(ind_keys[1]));
    fflush(stdout);
}

void
query_led_state(Bool ind_keys[2]) {
    unsigned int start_state;
    XkbGetIndicatorState(display, XkbUseCoreKbd, &start_state);
    if (ind_keys == NULL) {
        Bool ind_keys_once[2] = {False, False};
        display_ind_state(ind_keys_once, start_state);
    } else display_ind_state(ind_keys, start_state);
}

void
start_led_logger(Bool first) {
    Bool ind_keys[2] = {False, False};
    if (first) query_led_state(ind_keys);

    XkbSelectEvents(display, XkbUseCoreKbd, XkbIndicatorStateNotifyMask, XkbIndicatorStateNotifyMask);
    XkbIndicatorNotifyEvent event;
    while (True) {
        XNextEvent(display, (XEvent*) &event);
        if (event.xkb_type == XkbIndicatorStateNotify) display_ind_state(ind_keys, event.state);
    }
}

// vim:filetype=c
