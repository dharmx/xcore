VERCMD  ?= git describe --tags 2> /dev/null
VERSION := $(shell $(VERCMD) || cat ../VERSION)
NAME := xcore

CPPFLAGS += -D_POSIX_C_SOURCE=200809L -DVERSION=\"$(VERSION)\"
CFLAGS   += -std=c11 --optimize=3 -pedantic -Wall -Wextra -I/usr/include/X11
LDLIBS   := -lX11 -lm -lXi

PREFIX    ?= /usr/local
BINPREFIX ?= $(PREFIX)/bin
MANPREFIX ?= $(PREFIX)/share/man

SRC := $(wildcard *.c)
OBJ := $(SRC:.c=.o)
DEF := $(wildcard *.h)

all: $(NAME)

install: $(NAME)
	mkdir -p "$(DESTDIR)$(BINPREFIX)"
	cp -p $(NAME) "$(DESTDIR)$(BINPREFIX)"

uninstall:
	rm -f "$(DESTDIR)$(BINPREFIX)/$(NAME)"

debug: build
debug: CFLAGS += -O0 -g
debug:
	$(CC) -o $(NAME) $(OBJ) $(CFLAGS) $(LDLIBS)

$(NAME): build
	$(CC) -o $(NAME) $(OBJ) $(CFLAGS) $(LDLIBS)

build:
	$(CC) -c $(shell printf "%s\n" $(strip $1) | tac)$(SRC)

doc:
	@echo Coming Soon...

clean:
	rm -f $(OBJ) $(NAME)

format:
	clang-format -i $(SRC) $(DEF)

.PHONY: clean format doc build install uninstall debug all

# vim:filetype=make
