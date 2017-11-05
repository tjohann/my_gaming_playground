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
load_texture(char *file, SDL_Renderer *r)
{
	SDL_Surface *tmp = IMG_Load(file);
	if (tmp == NULL)
		err_sdl_and_ret("could not load file", NULL);

	SDL_Texture *t = SDL_CreateTextureFromSurface(r, tmp);
	if (t == NULL)
		err_sdl_and_ret("could not create texture", NULL);

	SDL_FreeSurface(tmp);

	return t;
}

LIGGAME_EXPORT void
destroy_texture(SDL_Texture *t)
{
	if (t != NULL)
		SDL_DestroyTexture(t);
	else
		printf("texture == NULL\n");
}

LIGGAME_LOCAL int
alloc_textures_via_config(config_t *cfg, SDL_Renderer *r,
			game_texture_t *t, size_t s)
{
	config_setting_t *setting = config_lookup(cfg, "config.textures");
	if (setting == NULL)
		err_and_ret("no textures entry in configuration", NULL);

	int count = config_setting_length(setting);

	/*
	 * TODO: NEXT
	 */


	return -1;
}
