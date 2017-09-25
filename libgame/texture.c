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

LIGGAME_EXPORT game_texture_cont_t *
load_texture_via_config(config_t *cfg, SDL_Renderer *renderer)
{
	game_texture_cont_t *c = malloc(sizeof(game_texture_cont_t));
	if (c == NULL)
		err_and_ret("could not alloc memory", NULL);

	config_setting_t *setting = config_lookup(cfg, "config.textures");
	if (setting != NULL)
	{
		int count = config_setting_length(setting);

		game_texture_t *t = malloc(count * sizeof(game_texture_t));
		if (t == NULL)
			err_and_ret("could not alloc memory", NULL);

		memset(t, 0, sizeof(*t));

		for(int i = 0; i < count; i++)
		{
			config_setting_t *texture =
				config_setting_get_elem(setting, i);

			const char *name, *file;
			if (!(config_setting_lookup_string(texture, "name",
									&name)
					&& config_setting_lookup_string(texture, "file",
									&file)))
				continue;

			t[count].name = alloc_string(name);
			t[count].file = alloc_string(file);
			t[count].texture = load_texture(t[count].file,
							renderer);
			if (t[count].texture == NULL)
				err_and_ret("could not create texture", NULL);

			//printf("addr t[count].name %p\n", t[count].name);
			printf("t[count].name %s\n", t[count].name);
			printf("t[count].file %s\n", t[count].file);

			//printf("addr of t[0].name %s\n", t[0].name);
			printf("t[0].name %p\n", t[0].name);
			printf("t[0].file %s\n", t[0].file);
		}

		printf("%p ....\n", t);

		c->array = t;
		c->count = count;
	} else {
		printf("no textures entry in config\n");
		return NULL;
	}

	printf("c->array[0].name %s\n", c->array[0].name);

	return c;
}

void
draw_texture(game_obj_data_t *obj, SDL_Renderer *renderer)
{
	SDL_Rect src_rect;
	SDL_Rect dest_rect;

	src_rect.x = 0;
	src_rect.y = 0;

	src_rect.w = dest_rect.w = obj->size.w;
	src_rect.h = dest_rect.h = obj->size.h;

	dest_rect.x = (uint32_t) obj->pos.x;
	dest_rect.y = (uint32_t) obj->pos.y;

	int err = SDL_RenderCopyEx(renderer, obj->texture,
				   &src_rect, &dest_rect,
				   0, 0, obj->flip);
	if (err < 0)
		eprintf("could not set render texture (%s)\n",
			SDL_GetError());
}

void
draw_frame_texture(game_obj_data_t *obj, SDL_Renderer *renderer)
{
	SDL_Rect src_rect;
	SDL_Rect dest_rect;

	src_rect.x = obj->size.w  * obj->frame;
	src_rect.y = 0;

	src_rect.w = dest_rect.w = obj->size.w;
	src_rect.h = dest_rect.h = obj->size.h;

	dest_rect.x = (uint32_t) obj->pos.x;
	dest_rect.y = (uint32_t) obj->pos.y;

	int err = SDL_RenderCopyEx(renderer, obj->texture,
				   &src_rect, &dest_rect,
				   0, 0, obj->flip);
	if (err < 0)
		eprintf("could not set render texture (%s)\n",
			SDL_GetError());
}
