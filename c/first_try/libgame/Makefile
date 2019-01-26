#
# build libgame.a
#

CC = gcc
CFLAGS = -std=gnu11 -Wall -Wextra `pkg-config --cflags sdl2 SDL2_image libconfig libxml-2.0`
LDLIBS =

LIB = libgame.a
OBJ = helper.o vector_2d.o window.o input.o texture.o objects.o collision.o ai.o object_fab.o object_fab_xml.o

%.o: %.c
	$(CC) -g -c -o $@ $< $(CFLAGS) $(LDLIBS)

$(LIB): $(OBJ)
	ar rcs $@ $(OBJ)

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
