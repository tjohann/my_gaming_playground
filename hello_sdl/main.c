/*
  GPL
  (c) 2017, thorsten.johannvorderbrueggen@t-online.de

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
*/

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PROGNAME "my first SDL window"

SDL_Window *window;
SDL_Renderer *renderer;

int
init_main_window(uint32_t flags)
{
	int err = SDL_Init(SDL_INIT_EVERYTHING);
	if (err < 0) {
		fprintf(stderr, "could not init SDL (%s)\n",
			SDL_GetError());
		return -1;
	}

	window = SDL_CreateWindow(PROGNAME,
				SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				1024, 768,
				flags);
	if (window == NULL) {
		fprintf(stderr, "could not create main window (%s)\n",
			SDL_GetError());
		return -1;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == NULL) {
		fprintf(stderr, "could not create renderer (%s)\n",
			SDL_GetError());
		return -1;
	}

	return 0;
}

int
render_main_window()
{
	/* set color R,G,B and alpha */
	int err = SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	if (err < 0) {
		fprintf(stderr, "could not set color (%s)\n",
			SDL_GetError());
		return -1;
	}

	/* clear rendering target with the drawing color */
	err = SDL_RenderClear(renderer);
	if (err < 0) {
		fprintf(stderr, "could not set clear rendering target (%s)\n",
			SDL_GetError());
		return -1;
	}

	/* bring everthing to the window -> now we see the changes */
	SDL_RenderPresent(renderer);

	return 0;
}


int
main(int argc, char *argv[])
{
	printf("start to try to build a SDL window\n");

	printf("usage: ./hello_sdl [-f]  \n");
	printf("       -f -> fullscreen  \n");

	int c;
	uint32_t flag = SDL_WINDOW_SHOWN;
	while ((c = getopt(argc, argv, "f")) != -1) {
		switch (c) {
		case 'f':
			flag = SDL_WINDOW_FULLSCREEN;
			printf("use SDL_WINDOW_FULLSCREEN\n");
			break;
		default:
			fprintf(stderr, "no valid option\n");
			exit(EXIT_FAILURE);
		}
	}

	if (init_main_window(flag) == -1) {
		fprintf(stderr, "could not setup a SDL window\n");
		exit(EXIT_FAILURE);
	}

	if (render_main_window() == -1) {
		fprintf(stderr, "could not render main window\n");
		exit(EXIT_FAILURE);
	}

	printf("sleep for 5 seconds\n");
	SDL_Delay(5000);
	SDL_Quit();

	printf("seams everthing went fine\n");

	exit(EXIT_SUCCESS);
}
