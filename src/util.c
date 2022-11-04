/* util.c
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

#include "util.h"

long
stol_wrap(const char* string) {
    int base   = string[1] == 'x' || string[1] == 'X' ? 16 : 10;
    char* end  = NULL;
    long value = strtol(string, &end, base);
    return value;
}

void
log_error(int return_status, const char* method) {
    switch (return_status) {
        case Success: return;
        case BadRequest: fprintf(stderr, "ERROR: Call to %s() resulted in a BadRequest\n", method); break;
        case BadValue: fprintf(stderr, "ERROR: Call to %s() resulted in a BadValue\n", method); break;
        case BadWindow: fprintf(stderr, "ERROR: Call to %s() resulted in a BadWindow\n", method); break;
        case BadPixmap: fprintf(stderr, "ERROR: Call to %s() resulted in a BadPixmap\n", method); break;
        case BadAtom: fprintf(stderr, "ERROR: Call to %s() resulted in a BadAtom\n", method); break;
        case BadCursor: fprintf(stderr, "ERROR: Call to %s() resulted in a BadCursor\n", method); break;
        case BadFont: fprintf(stderr, "ERROR: Call to %s() resulted in a BadFont\n", method); break;
        case BadMatch: fprintf(stderr, "ERROR: Call to %s() resulted in a BadMatch\n", method); break;
        case BadDrawable: fprintf(stderr, "ERROR: Call to %s() resulted in a BadDrawable\n", method); break;
        case BadAccess: fprintf(stderr, "ERROR: Call to %s() resulted in a BadAccess\n", method); break;
        case BadAlloc: fprintf(stderr, "ERROR: Call to %s() resulted in a BadAlloc\n", method); break;
        case BadColor: fprintf(stderr, "ERROR: Call to %s() resulted in a BadColor\n", method); break;
        case BadGC: fprintf(stderr, "ERROR: Call to %s() resulted in a BadGC\n", method); break;
        case BadIDChoice: fprintf(stderr, "ERROR: Call to %s() resulted in a BadIDChoice\n", method); break;
        case BadName: fprintf(stderr, "ERROR: Call to %s() resulted in a BadName\n", method); break;
        case BadLength: fprintf(stderr, "ERROR: Call to %s() resulted in a BadLength\n", method); break;
        case BadImplementation: fprintf(stderr, "ERROR: Call to %s() resulted in a BadImplementation\n", method); break;
    }
    exit(EXIT_FAILURE);
}

void
init_atoms(AtomMap* atoms, Bool only_if_exists) {
    atoms->_NET_WM_STATE_ADD    = 1;
    atoms->_NET_WM_STATE_REMOVE = 0;
    atoms->_NET_WM_STATE_TOGGLE = 2;
    atoms->_NO_ATOM             = None;

    atoms->_UTF8_STRING             = XInternAtom(display, "UTF8_STRING", only_if_exists);
    atoms->_CARDINAL                = XInternAtom(display, "CARDINAL", only_if_exists);
    atoms->_ATOM                    = XInternAtom(display, "ATOM", only_if_exists);
    atoms->_WINDOW                  = XInternAtom(display, "WINDOW", only_if_exists);
    atoms->_NET_ACTIVE_WINDOW       = XInternAtom(display, "_NET_ACTIVE_WINDOW", only_if_exists);
    atoms->_NET_DESKTOP_NAMES       = XInternAtom(display, "_NET_DESKTOP_NAMES", only_if_exists);
    atoms->_NET_WM_STATE            = XInternAtom(display, "_NET_WM_STATE", only_if_exists);
    atoms->_NET_WM_PID              = XInternAtom(display, "_NET_WM_PID", only_if_exists);
    atoms->_NET_NUMBER_OF_DESKTOPS  = XInternAtom(display, "_NET_NUMBER_OF_DESKTOPS", only_if_exists);
    atoms->_NET_WM_STATE_HIDDEN     = XInternAtom(display, "_NET_WM_STATE_HIDDEN", only_if_exists);
    atoms->_NET_WM_STATE_STICKY     = XInternAtom(display, "_NET_WM_STATE_STICKY", only_if_exists);
    atoms->_NET_WM_STATE_FULLSCREEN = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", only_if_exists);
    atoms->_WM_PROTOCOLS            = XInternAtom(display, "WM_PROTOCOLS ", only_if_exists);
}

Window
get_active_window() {
    Window window;
    int revert;
    XGetInputFocus(display, &window, &revert);
    return window;
}

WSize*
get_window_geometry(Drawable window) {
    Window root  = None;
    WSize* wsize = calloc(1, sizeof(WSize));
    int status   = XGetGeometry(display,
                              window,
                              &root,
                              &wsize->x_offset,
                              &wsize->y_offset,
                              &wsize->width,
                              &wsize->height,
                              &wsize->border,
                              &wsize->depth);

    wsize->window = window;
    wsize->root   = root;
    return wsize;
}

DesktopList*
get_desktops() {
    AtomMap EWMH;
    init_atoms(&EWMH, False);

    DesktopList* list = calloc(1, sizeof(DesktopList));
    Window root       = DefaultRootWindow(display);

    Atom type          = None;
    int format         = None;
    u_long bytes_after = None;

    XGetWindowProperty(display,
                       root,
                       EWMH._NET_DESKTOP_NAMES,
                       0,
                       32,
                       False,
                       EWMH._UTF8_STRING,
                       &type,
                       &format,
                       &list->nlabels,
                       &bytes_after,
                       &list->labels);
    return list;
}

long
get_total_desktops() {
    AtomMap EWMH;
    init_atoms(&EWMH, False);

    Window root        = DefaultRootWindow(display);
    Atom type          = None;
    int format         = None;
    u_long bytes_after = None;
    u_long nitems      = None;
    u_char* data       = None;

    XGetWindowProperty(display,
                       root,
                       EWMH._NET_NUMBER_OF_DESKTOPS,
                       0,
                       32,
                       False,
                       EWMH._CARDINAL,
                       &type,
                       &format,
                       &nitems,
                       &bytes_after,
                       &data);
    return *(long*) data;
}

long
get_window_pid(Window window) {
    AtomMap EWMH;
    init_atoms(&EWMH, False);

    Atom type          = None;
    int format         = None;
    u_long bytes_after = None;
    u_long nitems      = None;
    u_char* data       = None;

    XGetWindowProperty(display,
                       window,
                       EWMH._NET_WM_PID,
                       0,
                       32,
                       False,
                       EWMH._CARDINAL,
                       &type,
                       &format,
                       &nitems,
                       &bytes_after,
                       &data);
    return *(long*) data;
}

WProtocols*
get_window_protocols(Window window) {
    WProtocols* protocols = calloc(1, sizeof(WProtocols));
    XGetWMProtocols(display, window, &protocols->protocols, &protocols->count);
    return protocols;
}

XTextProperty*
get_window_title(Window window) {
    XTextProperty* title = calloc(1, sizeof(XTextProperty));
    XGetWMName(display, window, title);
    return title;
}

u_long
get_window_border(Window window) {
    XWindowAttributes attrs;
    XGetWindowAttributes(display, window, &attrs);
    return attrs.border_width;
}

char*
get_keyname_from_code(KeyCode code) {
    KeySym symbol = XkbKeycodeToKeysym(display, code, None, None);
    return XKeysymToString(symbol);
}

void
display_window_geometry(Window window, Bool shell) {
    WSize* wsize = get_window_geometry(window);
    if (shell) {
        fprintf(stdout,
                "WINDOW=0x%lx\n"
                "X=%d\nY=%d\n"
                "WIDTH=%u\n"
                "HEIGHT=%u\n"
                "BORDER=%u\n"
                "DEPTH=%u\n",

                wsize->window,
                wsize->x_offset,
                wsize->y_offset,
                wsize->width,
                wsize->height,
                wsize->border,
                wsize->depth);
    } else {
        fprintf(stdout,
                "window_geometry %d %d %u %u %u %u %ux%u+%d+%d 0x%lx\n",
                wsize->x_offset,
                wsize->y_offset,
                wsize->width,
                wsize->height,
                wsize->border,
                wsize->depth,
                wsize->width,
                wsize->height,
                wsize->x_offset,
                wsize->y_offset,
                wsize->window);
    }
    free(wsize);
}

void
display_desktops(char* delimiter) {
    DesktopList* list = get_desktops();

    delimiter = delimiter != NULL ? delimiter : "\n";
    for (u_int index = 0; index < list->nlabels; ++index)
        if (list->labels[index] == 0x0) fprintf(stdout, "%s", delimiter);
        else fprintf(stdout, "%c", list->labels[index]);

    fprintf(stdout, "\n");
    XFree(list->labels);
    XFree(list);
}

void
display_window_class(Window window) {
    XClassHint* hint = XAllocClassHint();
    XGetClassHint(display, window, hint);
    fprintf(stdout, "window_class %s %s\n", hint->res_name, hint->res_class);
    XFree(hint);
}

void
display_window_protocols(Window window) {
    WProtocols* protocols = get_window_protocols(window);
    char* names[protocols->count];

    XGetAtomNames(display, protocols->protocols, protocols->count, names);
    for (int index = 0; index < protocols->count; ++index) fprintf(stdout, "%s\n", names[index]);

    XFree(protocols->protocols);
    XFree(protocols);
}

void
display_window_title(Window window) {
    XTextProperty* title = get_window_title(window);
    fprintf(stdout, "window_title %s %lu %s\n", XGetAtomName(display, title->encoding), title->nitems, title->value);
    XFree(&title->value);
}

void
set_pointer_centered(Drawable window) {
    WSize* wsize = get_window_geometry(window);
    int status   = XWarpPointer(display,
                              None,
                              wsize->window,
                              None,
                              None,
                              wsize->x_offset,
                              wsize->y_offset,
                              wsize->width / 2,
                              wsize->height / 2);
    free(wsize);
}

void
set_window_focus(Window window, int revert) {
    XSetInputFocus(display, window, revert, CurrentTime);
}

void
set_window_activate(Window window) {
    AtomMap EWMH;
    XEvent payload;
    init_atoms(&EWMH, False);

    payload.type                 = ClientMessage;
    payload.xclient.display      = display;
    payload.xclient.window       = window;
    payload.xclient.message_type = EWMH._NET_ACTIVE_WINDOW;
    payload.xclient.format       = 32;
    payload.xclient.data.l[0]    = EWMH._NET_WM_STATE_TOGGLE;
    payload.xclient.data.l[1]    = CurrentTime;

    XSendEvent(display, DefaultRootWindow(display), False, WM_STATE_MASK, &payload);
}

static void
init_xclient_event(Window window, Atom message_type, Bool state, AtomMap* EWMH, XClientMessageEvent* payload) {
    payload->format       = 32;
    payload->display      = display;
    payload->window       = window;
    payload->send_event   = True;
    payload->message_type = message_type;
    payload->type         = ClientMessage;
    payload->data.l[0]    = state ? EWMH->_NET_WM_STATE_ADD : EWMH->_NET_WM_STATE_REMOVE;
}

int
set_window_state(Window window, WState window_state, Bool state) {
    AtomMap EWMH;
    Atom* atoms;
    int status = -1;
    int mask   = WM_STATE_MASK;
    XEvent payload;

    init_atoms(&EWMH, False);
    switch (window_state) {
        default: fprintf(stdout, "WARNING: No such option!\n"); break;
        case MAPPED: status = XMapWindow(display, window); break;
        case UNMAPPED: status = XWithdrawWindow(display, window, False); break;
        case CLOSE: status = XDestroyWindow(display, window); break;
        case KILL: status = XKillClient(display, window); break;

        case HIDDEN:
            init_xclient_event(window, EWMH._NET_WM_STATE, state, &EWMH, &payload.xclient);
            payload.xclient.data.l[1] = EWMH._NET_WM_STATE_HIDDEN;
            status                    = XSendEvent(display, window, True, mask, &payload);
            break;
        case FULLSCREEN:
            init_xclient_event(window, EWMH._NET_WM_STATE, state, &EWMH, &payload.xclient);
            payload.xclient.data.l[1] = EWMH._NET_WM_STATE_FULLSCREEN;
            status                    = XSendEvent(display, window, True, mask, &payload);
            break;
        case STICKY:
            init_xclient_event(window, EWMH._NET_WM_STATE, state, &EWMH, &payload.xclient);
            payload.xclient.data.l[1] = EWMH._NET_WM_STATE_STICKY;
            status                    = XSendEvent(display, window, True, WM_STATE_MASK, &payload);
            break;
    }

    return status;
}

// vim:filetype=c
