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


LIGGAME_EXPORT game_obj_t **
alloc_player_objects_via_config(config_t *cfg, game_texture_t textures[])
{
	return alloc_objects_via_config(cfg, "players", textures);
}

LIGGAME_EXPORT game_obj_t **
alloc_static_objects_via_config(config_t *cfg, game_texture_t textures[])
{
	return alloc_objects_via_config(cfg, "objects", textures);
}

LIGGAME_EXPORT game_obj_t **
alloc_enemie_objects_via_config(config_t *cfg, game_texture_t textures[])
{
	return alloc_objects_via_config(cfg, "enemies", textures);
}

LIGGAME_EXPORT game_obj_t **
alloc_objects_via_config(config_t *cfg, char *section, game_texture_t textures[])
{
	const char prefix[] = "config.";
	size_t len = strlen(section) + strlen(prefix) + 1;
	char *str = malloc(len);
	if (str == NULL)
		goto error;

	memset(str, 0, len);
	strncat(str, prefix, strlen(prefix));
	strncat(str, section, strlen(section));

	config_setting_t *setting = config_lookup(cfg, str);
	if (setting == NULL)
		err_and_ret("no players entry in configuration", NULL);

	char *name_ = NULL;
	char *texture_ = NULL;

	int count = config_setting_length(setting);
	game_obj_t **objs = malloc((count + 1) * sizeof(game_obj_t *));
	if (objs == NULL)
		goto error;

	for(int i = 0; i < count; i++)
	{
		config_setting_t *elem =
			config_setting_get_elem(setting, i);

		const char *name, *texture_name;
		int x, y;
		if (!(config_setting_lookup_string(elem, "name", &name)
				&& config_setting_lookup_string(elem, "texture",
								&texture_name)
				&& config_setting_lookup_int(elem, "pos_x", &x)
				&& config_setting_lookup_int(elem, "pos_y", &y)
				))
			continue;

		printf("name: %s ... texture: %s\n", name, texture_name);
		printf("position (%d/%d)\n", x, y);

		name_ = alloc_string(name);
		if (name_ == NULL)
			goto error;

		texture_ = alloc_string(texture_name);
		if (texture_ == NULL)
			goto error;

		objs[i] = alloc_game_object_from_array(name_, texture_,
						x, y, textures);

		free(name_);
		free(texture_);
	}

	/* to point out the end of the array */
	objs[count] = NULL;

	if (str != NULL)
		free(str);

	return objs;
error:
	eprintf("an error in %s occured\n", __FUNCTION__);

	if (name_ != NULL)
		free(name_);

	if (texture_ != NULL)
		free(texture_);

	if (str != NULL)
		free(str);

	if (objs != NULL) {
		for (int i = 0; objs[i] != NULL; i++)
			free_game_object(objs[i]);

		free(objs);
	}

	return NULL;
}
