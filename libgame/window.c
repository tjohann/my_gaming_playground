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

#include "libgame.h"
#include "libgame_private.h"


LIGGAME_EXPORT SDL_Window *
setup_main_window(const char *name, spread_t *screen, unsigned char flags)
{
	int err = SDL_Init(SDL_INIT_EVERYTHING);
	if (err < 0)
		err_sdl_and_ret("could not init SDL", NULL);

	/*
	 * TODO: use flags
	 */
	switch(flags) {

	default:
		printf("use default settings\n");
		flags = SDL_WINDOW_SHOWN;
	}
	SDL_Window *window = SDL_CreateWindow(name,
					SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED,
					screen->w, screen->h,
					flags);
	if (window == NULL)
		err_sdl_and_ret("could not create main window", NULL);

	return window;
}

LIGGAME_EXPORT SDL_Renderer *
setup_renderer(SDL_Window *window, color_t *b)
{
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
						SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
		err_sdl_and_ret("could not create renderer", NULL);

	int err = -1;
	if (b == NULL)
		err = SDL_SetRenderDrawColor(renderer, 0, 150, 0, 255);
	else
		err = SDL_SetRenderDrawColor(renderer, b->r, b->g, b->b, b->a);

	if (err < 0)
		err_sdl_and_ret("could not set color", NULL);

	return renderer;
}

LIGGAME_EXPORT void
cleanup_main_window(SDL_Window *window, SDL_Renderer *renderer)
{
	printf("cleanup all SDL related stuff\n");

	if (renderer != NULL)
		SDL_DestroyRenderer(renderer);
	else
		printf("renderer == NULL\n");

	if (window != NULL)
		SDL_DestroyWindow(window);
	else
		printf("window == NULL\n");

	SDL_Quit();
}
