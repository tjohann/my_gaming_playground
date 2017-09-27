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


/*
 * --------------------------- debugging stuff ---------------------------------
 */

LIGGAME_EXPORT void
show_object_kine_vals(game_obj_data_t *obj)
{
	printf("-----------------------------------\n");
	printf("position x/y %d/%d\n", obj->pos.x, obj->pos.y);
	printf("veloctiy x/y %d/%d\n", obj->velo.x,  obj->velo.y);
	printf("-----------------------------------\n\n");
}

LIGGAME_EXPORT void
show_object_size_vals(game_obj_data_t *obj)
{
	printf("-----------------------------------\n");
	printf("weight %d\n", obj->size.w);
	printf("height %d\n", obj->size.h);
	printf("-----------------------------------\n\n");
}

LIGGAME_EXPORT void
show_object_vals(game_obj_t *obj)
{
	show_object_kine_vals(obj->data);
	show_object_size_vals(obj->data);
}

/*
 * --------------------------- "string" stuff ----------------------------------
 */

LIGGAME_EXPORT char *
alloc_string(const char *s)
{
	char *str = NULL;
	size_t len = 0;

	if (s == NULL)
		return NULL;

	len = strlen(s) + 1;

	str = malloc(len);
	if (str == NULL)
		return NULL;

	memset(str, 0, len);
	strncat(str, s, len);

	return str;
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
