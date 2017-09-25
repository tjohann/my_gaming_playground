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

LIGGAME_EXPORT int
load_texture_via_config(config_t *cfg, game_texture_t *array[],
			SDL_Renderer *renderer)
{
	config_setting_t *setting = config_lookup(cfg, "config.textures");
	if (setting == NULL) {
		printf("no textures entry in config\n");
		return -1;
	}

	int count = config_setting_length(setting);

	size_t len = count * sizeof(game_texture_t *);
	game_texture_t **a = malloc(len);
	if (a == NULL)
		err_and_ret("could not alloc memory", -1);

	memset(a, 0, len);
	printf("len %ld \n", len);

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

		game_texture_t *t = malloc(sizeof(game_texture_t));
		if (t == NULL)
			err_and_ret("could not alloc memory", -1);

		t->name = alloc_string(name);
		t->texture = load_texture(file, renderer);
		if (t->texture == NULL)
			err_and_ret("could not create texture", -1);

		a[i] = t;
	}

	array = a;

	printf("sdfsfsd %s\n", array[0]->name);
	printf("sdfsfsd %p\n", array[1]->name);

	return 0;
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
