/*
  GPL
  (c) 2017-2019, thorsten.johannvorderbrueggen@t-online.de

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
setup_window(const char *name, spread_t *screen, unsigned int flags)
{
	/*
	  Taken from SDL documentation of SDL_CreateWindow()

	  flags:

	  SDL_WINDOW_FULLSCREEN         -> fullscreen window
	  SDL_WINDOW_FULLSCREEN_DESKTOP -> fullscreen window at the current
	                                   desktop resolution
	  SDL_WINDOW_OPENGL             -> window usable with OpenGL context
	  SDL_WINDOW_HIDDEN             -> window is not visible
	  SDL_WINDOW_BORDERLESS         -> no window decoration
	  SDL_WINDOW_RESIZABLE          -> window can be resized
	  SDL_WINDOW_MINIMIZED          -> window is minimized
	  SDL_WINDOW_MAXIMIZED          -> window is maximized
	  SDL_WINDOW_INPUT_GRABBED      -> window has grabbed input focus
	  SDL_WINDOW_ALLOW_HIGHDPI      -> window should be created in high-DPI
	                                   mode if supported (>= SDL 2.0.1)
	 */
	int err = SDL_Init(SDL_INIT_EVERYTHING);
	if (err < 0)
		err_sdl_and_ret("could not init SDL", NULL);

	SDL_Window *w = SDL_CreateWindow(name,
					SDL_WINDOWPOS_CENTERED,
					SDL_WINDOWPOS_CENTERED,
					screen->w, screen->h,
					flags);
	if (w == NULL)
		err_sdl_and_ret("could not create main window", NULL);

	return w;
}

LIGGAME_EXPORT SDL_Renderer *
setup_renderer(SDL_Window *w, color_t *b, unsigned int flags)
{
        /*
	  Taken from SDL documentation of SDL_CreateRenderer()

	  possible flags:

	  SDL_RENDERER_SOFTWARE      -> the renderer is a software fallback
	  SDL_RENDERER_ACCELERATED   -> the renderer uses hardware acceleration
	  SDL_RENDERER_PRESENTVSYNC  -> present is synchronized with the refresh
	                                rate
	  SDL_RENDERER_TARGETTEXTURE -> the renderer supports rendering to
	                                texture
	 */

	SDL_Renderer *r = SDL_CreateRenderer(w, -1, flags);
	if (r == NULL)
		err_sdl_and_ret("could not create renderer", NULL);

	int err = -1;
	if (b == NULL)
		err = SDL_SetRenderDrawColor(r, 0, 150, 0, 255);
	else
		err = SDL_SetRenderDrawColor(r, b->r, b->g, b->b, b->a);

	if (err < 0)
		err_sdl_and_ret("could not set color", NULL);

	return r;
}

LIGGAME_EXPORT void
cleanup_window(SDL_Window *w, SDL_Renderer *r)
{
	printf("cleanup all SDL related stuff\n");

	if (r != NULL)
		SDL_DestroyRenderer(r);
	else
		eprintf("renderer == NULL\n");

	if (w != NULL)
		SDL_DestroyWindow(w);
	else
		eprintf("window == NULL\n");

	SDL_Quit();
}

/*
 * --------------------------- config related ----------------------------------
 */

LIGGAME_LOCAL int
setup_window_via_config(config_t *cfg, game_t *game)
{
	const char *str;
	int w = 0, h = 0;

	if (!config_lookup_string(cfg, "config.window.title", &str))
		err_and_ret("config.window.title not available", -1);

	if (!config_lookup_int(cfg, "config.window.size.w", &w))
		err_and_ret("config.window.size.w not available", -1);

	if (!config_lookup_int(cfg, "config.window.size.h", &h))
		err_and_ret("config.window.size.h not available", -1);

	game->w = w;
	game->h = h;

	/*
	 * TODO: handle flags from configuration or set defaults
	 */
	unsigned int flags = 0;
	flags |= SDL_WINDOW_SHOWN;

	game->window = setup_window(str, &game->screen, flags);
	if (game->window == NULL)
		return -1;
	else
		return 0;
}

LIGGAME_LOCAL int
setup_renderer_via_config(config_t *cfg, game_t *game)
{
	color_t b;
	memset(&b, 0, sizeof(b));

	int tmp = 0;

	if (!config_lookup_int(cfg, "config.window.background.r", &tmp)) {
		eprintf("config.window.background.r not available\n");
		return -1;
	}
	b.r = tmp;

	if (!config_lookup_int(cfg, "config.window.background.g", &tmp)) {
		eprintf("config.window.background.g not available\n");
		return -1;
	}
	b.g = tmp;

	if (!config_lookup_int(cfg, "config.window.background.b", &tmp)) {
		eprintf("config.window.background.b not available\n");
		return -1;
	}
	b.b = tmp;

	if (!config_lookup_int(cfg, "config.window.background.a", &tmp)) {
		eprintf("config.window.background.a not available\n");
		return -1;
	}
	b.a = tmp;

	/*
	 * TODO: handle flags from configuration or set defaults
	 */
	unsigned int flags = 0;
	flags |= SDL_RENDERER_ACCELERATED;

	game->renderer = setup_renderer(game->window, &b, flags);
	if (game->renderer == NULL)
		return -1;
	else
		return 0;
}
