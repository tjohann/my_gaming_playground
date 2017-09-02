#
# build libgame.a
#

CC = gcc
CFLAGS = -std=gnu11 -Wall -Wextra `pkg-config --cflags sdl2 SDL2_image`
#LDLIBS = `pkg-config --libs sdl2 SDL2_image`
LDLIBS =

LIB = libgame.a
OBJ = libgame.o

%.o: %.c
	$(CC) -g -c -o $@ $< $(CFLAGS) $(LDLIBS)

$(LIB): $(OBJ)
	ar rcs $@ $<

install: $(LIB)
	install $(LIB) $(HOME)/lib/$(LIB)

uninstall:
	rm -rf $(HOME)/lib/$(LIB)

clean:
	rm -rf *~
	rm -rf .*~
	rm -rf *.o
	rm -rf $(LIB)

distclean: clean

.PHONY: clean install uninstall
