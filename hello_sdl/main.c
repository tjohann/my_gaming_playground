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

#define PROGNAME "my first SDL window"

SDL_Window *window;
SDL_Renderer *renderer;

int
init_main_window()
{
	int err = SDL_Init(SDL_INIT_EVERYTHING);
	if (err == -1) {
		fprintf(stderr, "could not init SDL\n");
		return -1;
	}

	window = SDL_CreateWindow(PROGNAME,
				SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				1024, 768,
				SDL_WINDOW_SHOWN);
	if (window == NULL) {
		fprintf(stderr, "could not create main window\n");
		return -1;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == NULL) {
		fprintf(stderr, "could not create renderer\n");
		return -1;
	}

	return 0;
}

int
render_main_window()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	return 0;
}


int
main(int argc, char *argv[])
{
	printf("Hello SDL ... try to build a SDL window\n");

	if (init_main_window() == -1) {
		fprintf(stderr, "could not setup a SDL window");
		exit(EXIT_FAILURE);
	}

	if (render_main_window() == -1) {
		fprintf(stderr, "could not render main window");
		exit(EXIT_FAILURE);
	}

	SDL_Delay(5000);
	SDL_Quit();

	printf("seams everthing went fine\n");

	exit(EXIT_SUCCESS);
}
