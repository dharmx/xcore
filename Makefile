all: compile

install: compile
	cp xcore ~/.local/bin/xcore

compile: build
	$(CC) -o xcore xcore.o -I/usr/include/X11 -lX11

build: xcore.c ansi-colors.h
	$(CC) -c xcore.c

clean:
	rm xcore *.o

format:
	clang-format -i *.c *.h
