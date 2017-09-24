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
setup_main_window_via_config(config_t *cfg, unsigned char flags)
{
	const char *str;
	int w = 0, h = 0;

	if (!config_lookup_string(cfg, "config.window.title", &str)) {
		eprintf("config.window.title not available\n");
		return NULL;
	}
	if (!config_lookup_int(cfg, "config.window.size.w", &w)) {
		eprintf("config.window.size.w not available\n");
		return NULL;
	}
	if (!config_lookup_int(cfg, "config.window.size.h", &h)) {
		eprintf("config.window.size.h not available\n");
		return NULL;
	}

	return setup_main_window(str, w, h, flags);
}

LIGGAME_EXPORT SDL_Window *
setup_main_window(const char *name, uint32_t size_x, uint32_t size_y, unsigned char flags)
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
					      size_x, size_y,
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

LIGGAME_EXPORT SDL_Renderer *
setup_renderer_via_config(config_t *cfg, SDL_Window *window)
{
	color_t t;
	memset(&t, 0, sizeof(t));

	int tmp = 0;

	if (!config_lookup_int(cfg, "config.window.background.r", &tmp)) {
		eprintf("config.window.background.r not available\n");
		return NULL;
	}
	t.r = tmp;

	if (!config_lookup_int(cfg, "config.window.background.g", &tmp)) {
		eprintf("config.window.background.g not available\n");
		return NULL;
	}
	t.g = tmp;

	if (!config_lookup_int(cfg, "config.window.background.b", &tmp)) {
		eprintf("config.window.background.b not available\n");
		return NULL;
	}
	t.b = tmp;

	if (!config_lookup_int(cfg, "config.window.background.a", &tmp)) {
		eprintf("config.window.background.a not available\n");
		return NULL;
	}
	t.a = tmp;

	return setup_renderer(window, &t);
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
