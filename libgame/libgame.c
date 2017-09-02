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

#define LIGGAME_EXPORT __attribute__ ((visibility ("default")))

#define err_sdl_and_ret(err_txt, ret_val) do {			\
		fprintf(stderr, err_txt " (%s)\n",		\
			SDL_GetError());			\
		return ret_val;					\
	}							\
	while(0)

#define err_and_ret(err_txt, ret_val) do {			\
		fprintf(stderr, err_txt "\n");			\
		return ret_val;					\
	}							\
	while(0)

#define err_and_ret(err_txt, ret_val) do {			\
		fprintf(stderr, err_txt "\n");			\
		return ret_val;					\
	}							\
	while(0)


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
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
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

LIGGAME_EXPORT SDL_Texture *
load_texture(char *file_name, SDL_Renderer *renderer)
{
	if (file_name == NULL)
		;
	else
		printf("create texture from %s\n", file_name);

	SDL_Surface *tmp = IMG_Load(file_name);
	if (tmp == NULL)
		err_sdl_and_ret("could not load png", NULL);

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, tmp);
	if (texture == NULL)
		err_sdl_and_ret("could not create texture", NULL);

	SDL_FreeSurface(tmp);

	return texture;
}

LIGGAME_EXPORT void
draw_texture(SDL_Texture *texture, SDL_Renderer *renderer,
	     pos_t pos, SDL_RendererFlip flip)
{
	SDL_Rect src_rect;
	SDL_Rect dest_rect;

	src_rect.x = 0;
	src_rect.y = 0;

	src_rect.w = dest_rect.w = pos.w;
	src_rect.h = dest_rect.h = pos.h;

	dest_rect.x = pos.x;
	dest_rect.y = pos.y;

	int err = SDL_RenderCopyEx(renderer, texture,
				   &src_rect, &dest_rect,
				   0, 0, flip);
	if (err < 0)
		eprintf("could not set render texture (%s)\n",
			SDL_GetError());
}

LIGGAME_EXPORT void
draw_frame_texture(SDL_Texture *texture, SDL_Renderer *renderer,
		   pos_t pos, unsigned char frame, SDL_RendererFlip flip)
{
	SDL_Rect src_rect;
	SDL_Rect dest_rect;

	src_rect.x = pos.w  * frame;
	src_rect.y = 0;

	src_rect.w = dest_rect.w = pos.w;
	src_rect.h = dest_rect.h = pos.h;

	dest_rect.x = pos.x;
	dest_rect.y = pos.y;

	int err = SDL_RenderCopyEx(renderer, texture,
				   &src_rect, &dest_rect,
				   0, 0, flip);
	if (err < 0)
		eprintf("could not set render texture (%s)\n",
			SDL_GetError());
}
