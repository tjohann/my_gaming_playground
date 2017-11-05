#
# build example
#

CC = gcc
CFLAGS = -std=gnu11 -Wall -Wextra `pkg-config --cflags sdl2 SDL2_image libconfig libxml-2.0`
LDLIBS = -I../libgame -L../libgame -lgame -lm `pkg-config --libs sdl2 SDL2_image libconfig libxml-2.0`

PROG = simple_ai
OBJ = main.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(LDLIBS)

$(PROG): $(OBJ)
	gcc -o $@ $^ $(LDLIBS)

install: $(PROG)
	install $(PROG) $(HOME)/bin/$(PROG)

uninstall:
	rm -rf $(HOME)/bin/$(PROG)

clean:
	rm -rf *~
	rm -rf .*~
	rm -rf *.o
	rm -rf $(PROG)

distclean: clean

.PHONY: clean uninstall
