/*
  GPL
  (c) 2019, thorsten.johannvorderbrueggen@t-online.de

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
#include <SDL_log.h>
#include <SDL_image.h>

int
main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		exit(EXIT_FAILURE);
	} else {
		SDL_Log("SDL initialised");
	}

	SDL_Window *window = SDL_CreateWindow("sdl_game",
					      SDL_WINDOWPOS_UNDEFINED,
					      SDL_WINDOWPOS_UNDEFINED,
					      640, 480,
					      SDL_WINDOW_SHOWN);
	if (window == 0) {
		SDL_Log("Unable to initialize window: %s",  SDL_GetError());
		exit(EXIT_FAILURE);
	} else {
		SDL_Log("Window initialised");
	}


        /*
         * -1 init the first on supporting
         * 0 (try first SDL_RENDERER_ACCELERATED)
         */
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == 0) {
		SDL_Log("Unable to initialize renderer: %s",  SDL_GetError());
		exit(EXIT_FAILURE);
	} else {
		SDL_Log("Renderer initialised");
	}

        // RGB and alpha -> set to black
        int ret = SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        if (ret != 0) {
		SDL_Log("Unable to set background color: %s",  SDL_GetError());
		exit(EXIT_FAILURE);
	} else {
		SDL_Log("Set background color");
	}

        ret = SDL_RenderClear(renderer);
        if (ret != 0) {
		SDL_Log("Unable to clear window: %s",  SDL_GetError());
		exit(EXIT_FAILURE);
	} else {
		SDL_Log("Cleared window");
	}

        // show everything
        SDL_RenderPresent(renderer);

        SDL_Delay(5000);

        // cleanup
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
	SDL_Quit();

	exit(EXIT_SUCCESS);
}
