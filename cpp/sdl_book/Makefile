#
# build example
#

CC = g++
CFLAGS = -Wall -Wextra `pkg-config --cflags sdl2 SDL2_image`
LDLIBS = `pkg-config --libs sdl2 SDL2_image `

PROG = sdl_game
OBJ = main.o game.o texture_manager.o

%.o: %.cc
	$(CC) -c -o $@ $< $(CFLAGS) $(LDLIBS)

$(PROG): $(OBJ)
	$(CC) -o $@ $^ $(LDLIBS)

clean:
	rm -rf *~
	rm -rf .*~
	rm -rf *.o
	rm -rf $(PROG)

distclean: clean

.PHONY: clean
