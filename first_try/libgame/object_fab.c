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

#define SECTION_PLAYERS "players"
#define SECTION_ENEMIES "enemies"
#define SECTION_OBJECTS "objects"

LIGGAME_EXPORT game_obj_t **
alloc_objects_via_config(config_t *cfg,  char *section, game_texture_t textures[],
			unsigned char flags)
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
						x, y, textures, flags);

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


/*
 * --------------------------- window related ----------------------------------
 */

LIGGAME_EXPORT SDL_Window *
setup_main_window_via_config(config_t *cfg, unsigned char flags,
			spread_t *screen)
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

	screen->w = w;
	screen->h = h;

	return setup_main_window(str, screen, flags);
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


/*
 * --------------------------- input related -----------------------------------
 */

LIGGAME_EXPORT game_joystick_t *
alloc_joystick_objects_via_config(config_t *cfg)
{
	config_setting_t *setting = config_lookup(cfg, "config.joysticks");
	if (setting == NULL)
		err_and_ret("no joysticks entry in configuration", NULL);

	int count = config_setting_length(setting);
	unsigned char n = get_num_joysticks();

	if (n < count)
		count = n;

	size_t len = (count + 1) * sizeof(game_joystick_t);
	game_joystick_t *a = malloc(len);
	if (a == NULL)
		err_and_ret("could not alloc memory", NULL);
	memset(a, 0, len);

	SDL_Joystick *j[count];
	int err = init_joysticks(j);
	if (err == -1)
		goto error;

	for(int i = 0; i < count; i++)
	{
		config_setting_t *joystick =
			config_setting_get_elem(setting, i);

		const char *name, *player;
		int step;
		if (!(config_setting_lookup_string(joystick, "name", &name)
				&& config_setting_lookup_string(joystick,
								"player",
								&player)
				&& config_setting_lookup_int(joystick, "step",
							&step)))
			continue;

		a[i].name = alloc_string(name);
		a[i].player = alloc_string(player);
		a[i].step = step;
		a[i].joystick = j[i];
		a[i].to_change = NULL;
		a[i].handle_axis_joystick = NULL;
	}

	/* to point out the end of the array */
	a[count].name = NULL;
	a[count].player = NULL;
	a[count].step = 0;
	a[count].joystick = NULL;
	a[count].to_change = NULL;
	a[count].handle_axis_joystick = NULL;

	return a;
error:
	eprintf("an error in %s occured\n", __FUNCTION__);

	if (a != NULL)
		free_joystick_array(a);

	if (j != NULL)
		free_joysticks(j);


	return NULL;
}


/*
 * --------------------------- config stuff ------------------------------------
 */

LIGGAME_EXPORT int
open_config(char *file, char *name, config_t *cfg)
{
	config_init(cfg);

	if (!config_read_file(cfg, file)) {
		eprintf("could not read config file %s:%d - %s\n",
			config_error_file(cfg),
			config_error_line(cfg),
			config_error_text(cfg));
		return -1;
	}

	const char *str;
	if (!config_lookup_string(cfg, "name", &str)) {
		eprintf("no valid naming in configuration file\n");
		return -1;
	}

	if (strlen(str) != strlen(name)) {
		eprintf("not a valid configuration\n");
		return -1;
	}

	if (strncmp(str, name, strlen(name)) != 0) {
		eprintf("not a valid configuration\n");
		return -1;
	}

	return 0;
}


/*
 * --------------------------- texture related ---------------------------------
 */

LIGGAME_EXPORT game_texture_t *
alloc_textures_via_config(config_t *cfg, SDL_Renderer *renderer)
{
	config_setting_t *setting = config_lookup(cfg, "config.textures");
	if (setting == NULL)
		err_and_ret("no textures entry in configuration", NULL);

	int count = config_setting_length(setting);

	size_t len = (count + 1) * sizeof(game_texture_t);
	game_texture_t *a = malloc(len);
	if (a == NULL)
		err_and_ret("could not alloc memory", NULL);
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
		a[i].texture = load_texture(tmp, renderer);
		if (a[i].texture == NULL) {
			free(tmp);
			err_and_ret("could not create texture", NULL);
		}
		a[i].name = alloc_string(name);

		free(tmp);
	}

	/* to point out the end of the array */
	a[count].name = NULL;
	a[count].texture = NULL;

	return a;
}


/*
 * --------------------------- game related ------------------------------------
 */

LIGGAME_EXPORT int
init_game_via_config(game_t *game, unsigned char flags)
{
	config_t cfg;
	int err = open_config(game->config_file, game->progname, &cfg);
	if (err == -1)
		exit(EXIT_FAILURE);

	/* setup main window */
	game->window = setup_main_window_via_config(&cfg, 0, &game->screen);
	if (game->window == NULL)
		err_and_ret("could not setup main window", -1);

	game->renderer = setup_renderer_via_config(&cfg, game->window);
	if (game->renderer == NULL)
		err_and_ret("could not setup renderer", -1);

	/* load all textures */
	game->texture_array = alloc_textures_via_config(&cfg, game->renderer);
	if (game->texture_array == NULL)
		err_and_ret("could not alloc textures", -1);

	if (flags & INIT_PLAYERS) {
		game->players = alloc_objects_via_config(&cfg, "players",
							game->texture_array, flags);
		if (game->players == NULL)
			printf("no player objects allocated!\n");
		else
			printf("player objects allocated\n");
	} else {
		printf("no player objects\n");
	}

	if (flags & INIT_OBJECTS) {
		game->static_objs = alloc_objects_via_config(&cfg, "objects",
							game->texture_array, flags);
		if (game->static_objs == NULL)
			printf("no static objects allocated!\n");
		else
			printf("static objects allocated\n");

	} else {
		printf("no static objects\n");
	}

	if (flags & INIT_ENEMIES) {
		game->enemies = alloc_objects_via_config(&cfg, "enemies",
							game->texture_array, flags);
		if (game->enemies == NULL)
			printf("no enemie objects allocated!\n");
		else
			printf("enemy objects allocated\n");
	} else {
		printf("no enemy objects\n");
	}

	game->joystick_array = alloc_joystick_objects_via_config(&cfg);
	if ((game->joystick_array == NULL) && (flags & INIT_PLAYERS))
		printf("no joysticks allocated!\n");
	else
		connect_player_objects(game->joystick_array, game->players);

	config_destroy(&cfg);

	return 0;
}
