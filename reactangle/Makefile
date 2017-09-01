#
# build example
#

CC = gcc
CFLAGS = -std=gnu11 -Wall -Wextra `pkg-config --cflags sdl2`

PROG = reactangle
OBJ = main.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(PROG): $(OBJ)
	gcc -o $@ $^ `pkg-config --libs sdl2`

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
