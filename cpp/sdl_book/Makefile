#
# build example
#

CC = g++

# to add -Weffc++ -Wsign-conversion
CFLAGS = -Wall -Wextra -std=c++14 `pkg-config --cflags sdl2 SDL2_image`
LDLIBS = `pkg-config --libs sdl2 SDL2_image `

PROG = sdl_game
OBJ = main.o game.o sdl_game_object.o player.o texture_manager.o

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
