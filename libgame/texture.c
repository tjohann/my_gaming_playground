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
	if (t != NULL || s != 0)
		err_and_ret("game_texture_t != NULL || size_t s != 0", -1);

	config_setting_t *setting = config_lookup(cfg, "config.textures");
	if (setting == NULL)
		err_and_ret("no textures entry in configuration", -1);

	int count = config_setting_length(setting);

	size_t len = count * sizeof(game_texture_t);
	game_texture_t *a = malloc(len);
	if (a == NULL)
		err_and_ret("could not alloc memory", -1);
	memset(a, 0, len);

	for(int i = 0; i < count; i++)
	{
		config_setting_t *texture =
			config_setting_get_elem(setting, i);

		const char *name, *file;
		if (!(config_setting_lookup_string(texture, "name",
								&name)
				&& config_setting_lookup_string(texture,
								"file",
								&file)))
			continue;

		char *tmp = alloc_string(file); /* to satisfy -Wall/-Wextra */
		a[i].texture = load_texture(tmp, r);
		if (a[i].texture == NULL) {
			free(tmp);
			err_and_ret("could not create texture", -1);
		}
		a[i].name = alloc_string(name);

		free(tmp);
	}

	t = a;
	s = count;

	return 0;
}
