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


LIGGAME_EXPORT game_obj_t *
alloc_game_object(char *name, game_obj_type_t obj_type,
		int x, int y,
		SDL_Texture *texture)
{
	game_obj_t *obj = malloc(sizeof(game_obj_t));
	if (obj == NULL)
		goto error;
	memset(obj, 0, sizeof(game_obj_t));

	obj->name = alloc_string(name);
	if (obj->name == NULL)
		goto error;

	game_obj_data_t *data = malloc(sizeof(game_obj_data_t));
	if (data == NULL)
		goto error;

	memset(data, 0, sizeof(game_obj_data_t));

	game_obj_func_t *func = NULL;
	if (obj_type & STATIC_OBJECT) {
		printf("%s is a static object\n", obj->name);
	} else {
		printf("%s is NOT static object\n", obj->name);

		game_obj_func_t *func = malloc(sizeof(game_obj_func_t));
		if (func == NULL)
			goto error;

		memset(func, 0, sizeof(game_obj_func_t));
	}

	obj->act_data = data;
	obj->func = func;






	return obj;
error:
	if (func != NULL)
		free(func);

	if (data != NULL)
		free(data);

	if (obj != NULL) {
		if (obj->name != NULL)
			free(obj->name);
		free(obj);
	}

	return NULL;
}
