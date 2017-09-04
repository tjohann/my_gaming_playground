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
