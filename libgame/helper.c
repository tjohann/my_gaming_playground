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
setup_main_window(char *name, int size_x, int size_y, unsigned char f)
{
	int err = SDL_Init(SDL_INIT_EVERYTHING);
	if (err < 0)
		err_sdl_and_ret("could not init SDL", NULL);

	/*
	 * TODO: use flags
	 */
	uint32_t flags = 0;
	switch(f) {

	default:
		printf("use default settings\n");
		f = SDL_WINDOW_SHOWN;
	}
	SDL_Window *window = SDL_CreateWindow(name,
					      SDL_WINDOWPOS_CENTERED,
					      SDL_WINDOWPOS_CENTERED,
					      size_x, size_y,
					      flags);
	if (window == NULL)
		err_sdl_and_ret("could not create main window", NULL);

	return window;
}

LIGGAME_EXPORT SDL_Renderer *
setup_renderer(SDL_Window *window, char *background)
{
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
		err_sdl_and_ret("could not create renderer", NULL);

	/*
	 * TODO: background color (red, green, blue ...)
	 */
	int err = SDL_SetRenderDrawColor(renderer, 0, 150, 0, 255);
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

LIGGAME_EXPORT game_obj_t *
init_game_object(int x, int y, int w, int h, SDL_Texture *texture)
{
	game_obj_t *t = malloc(sizeof(game_obj_t));
	if (t == NULL)
		err_and_ret("could not alloc mem", NULL);

	t->pos.x = x;
	t->pos.y = y;
	t->pos.h = h;
	t->pos.w = w;

	t->frame = 0;

	t->texture = texture;
	t->flip = SDL_FLIP_NONE;

	return t;
}

LIGGAME_EXPORT void
free_game_object(game_obj_t *t)
{
	if (t != NULL)
		free(t);
	else
		printf("game object == NULL\n");
}
