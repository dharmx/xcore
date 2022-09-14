#include <X11/Xlib.h>
#include <getopt.h>

#include <stdio.h>
#include <stdlib.h>

#include "ansi-colors.h"

#define NIL (0)
#define VERSION "xcore-0.1.0"

Display* display;

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

Window get_active_window() {
    Window window;
    int revert;

    switch (XGetInputFocus(display, &window, &revert)) {
        case BadWindow:
            fprintf(stderr, "XGetInputFocus: BadWindow ERROR!\n");
            abort();
            break;
        case BadMatch:
            fprintf(stderr, "XGetInputFocus: BadMatch ERROR!\n");
            abort();
            break;
        case BadValue:
            fprintf(stderr, "XGetInputFocus: BadValue ERROR!\n");
            abort();
            break;
    }
    return window;
}

WSize* get_active_window_geometry(Drawable window) {
    Window root;
    WSize* wsize = calloc(1, sizeof(WSize));

    wsize->window = window;
    int status = XGetGeometry(
        display, window, &root, &wsize->x_offset, &wsize->y_offset, &wsize->width, &wsize->height, &wsize->border,
        &wsize->depth);
    wsize->root = root;

    if (status == BadDrawable) {
        fprintf(stderr, "XGetGeometry: BadDrawable ERROR!\n");
        abort();
    }
    return wsize;
}

void display_active_window_geometry(Bool shell) {
    WSize* wsize = get_active_window_geometry(get_active_window());
    if (shell) {
        fprintf(
            stdout, "WINDOW=0x%lx\nX=%d\nY=%d\nWIDTH=%u\nHEIGHT=%u\nBORDER=%u\nDEPTH=%u\n", wsize->window,
            wsize->x_offset, wsize->y_offset, wsize->width, wsize->height, wsize->border, wsize->depth);
    } else {
        fprintf(
            stdout, "node_geometry %d %d %u %u %u %u %ux%u+%d+%d 0x%lx\n", wsize->x_offset, wsize->y_offset,
            wsize->width, wsize->height, wsize->border, wsize->depth, wsize->width, wsize->height, wsize->x_offset,
            wsize->y_offset, wsize->window);
    }
    free(wsize);
}

void center_pointer(WSize* wsize) {
    int status = XWarpPointer(
        display, None, wsize->window, None, None, wsize->x_offset, wsize->y_offset, wsize->width / 2,
        wsize->height / 2);
    if (status == BadWindow) {
        fprintf(stderr, "XWarpPointer: BadWindow ERROR!\n");
        abort();
    }
    free(wsize);
}

void pointer_follows_focus() {
    while (True) {
        XEvent xevent;
        XNextEvent(display, &xevent);
        switch (xevent.type) {
            case MapNotify: fprintf(stdout, "Mapped!\n"); break;
            case UnmapNotify: fprintf(stdout, "Unmapped!\n"); break;
            case DestroyAll: fprintf(stdout, "DestroyAll!\n"); break;
            case RetainPermanent: fprintf(stdout, "RetainPermanent!\n"); break;
            case RetainTemporary: fprintf(stdout, "RetainTemporary!\n"); break;
            default: fprintf(stdout, "%d!\n", xevent.type); break;
        }
    }
}

void display_help_message() {
    fprintf(
        stdout,
        "%sxcore 0.1.0"
        "\ndharmx\n"
        "\n%sUSAGE:%s"
        "\n\txcore -[a|x|g|c|v|d|e|h]%s\n"
        "\nOPTIONS:%s"
        "\n\t-a, --active           print active wid"
        "\n\t-g, --geometry         active window geometry format: x y w h border depth wxh+x+y wid"
        "\n\t-x, --geometry-sh      similar to -g but can be used with shell eval"
        "\n\t-c, --center           move pointer to the middle of the window"
        "\n\t-v, --version          print the version number"
        "\n\t-l, --loop             pointer follow focus loop [testing]"
        "\n\t-h, --help             print this message\n\n%s",
        GRN, BLU, MAG, BLU, MAG, RESET);
}

// cc xcore.c -o xcore -I/usr/include/X11 -lX11
int main(int argc, char** argv) {
    display = XOpenDisplay(None);

    int option_index;
    int option;
    struct option long_options[] = {
        {"active", no_argument, 0, 'a'}, {"geometry-sh", no_argument, 0, 'x'}, {"geometry", no_argument, 0, 'g'},
        {"center", no_argument, 0, 'c'}, {"version", no_argument, 0, 'v'},     {"loop", no_argument, 0, 'l'},
        {"help", no_argument, 0, 'h'},
    };

    while ((option = getopt_long(argc, argv, ":axgcvdlh", long_options, &option_index)) != -1) {
        switch (option) {
            case 'a': printf("0x%lx\n", get_active_window()); break;
            case 'x': display_active_window_geometry(True); break;
            case 'g': display_active_window_geometry(False); break;
            case 'c': center_pointer(get_active_window_geometry(get_active_window())); break;
            case 'v': fprintf(stdout, "%s\n", VERSION); break;
            case 'l': pointer_follows_focus(); break;
            case 'h':
            case '?': display_help_message(); break;
        }
    }

    XFlush(display);
    XCloseDisplay(display);

    if (argc == 1)
        display_help_message();
    return 0;
}

// vim:filetype=c
