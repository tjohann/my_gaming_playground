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


LIGGAME_LOCAL game_obj_data_t *
alloc_game_object_data(int x, int y, int w, int h,
		       int max_frames_x, int max_frames_y)
{
	game_obj_data_t *t = malloc(sizeof(game_obj_data_t));
	if (t == NULL)
		err_and_ret("could not alloc mem for game_obj_data_t *t", NULL);
	memset(t, 0, sizeof(game_obj_data_t));

	t->size.w = w;
	t->size.h = h;

	t->pos.x = x;
	t->pos.y = y;

	t->frame_x = 1;
	t->max_frames_x = max_frames_x;

	t->frame_y = 1;
	t->max_frames_y = max_frames_y;

	t->flip = SDL_FLIP_NONE;

	return t;
}

LIGGAME_EXPORT game_obj_t *
alloc_game_object(char *name, int x, int y,  game_texture_t *t)
{
	game_obj_t *obj = malloc(sizeof(game_obj_t));
	if (obj == NULL)
		return NULL;
	memset(obj, 0, sizeof(game_obj_t));

	obj->data = alloc_game_object_data(x, y, t->size.w, t->size.h,
					   t->max_frames_x, t->max_frames_y);
	if (obj->data == NULL)
		goto error;

	obj->name = alloc_string(name);
	if (obj->name == NULL)
		goto error;

	obj->texture = t->texture;
	obj->func = NULL;
	obj->event_data = NULL;

	return obj;
error:
	if (obj->data != NULL)
		free(obj->data);

	if (obj->name != NULL)
		free(obj->name);
	free(obj);

	err_and_ret("could not alloc game object", NULL);
}

LIGGAME_EXPORT void free_game_object(game_obj_t *t)
{
	if (t == NULL) {
		printf("nothing to free in %s", __FUNCTION__);
	} else {
		if (t->data != NULL)
			free(t->data);
		if (t->func != NULL)
			free(t->func);
		if (t->event_data != NULL)
			free(t->event_data);

		if (t->name != NULL)
			free(t->name);
		free(t);
	}
}

LIGGAME_EXPORT void
draw_game_object(game_obj_t *t, SDL_Renderer *renderer)
{
	draw_texture(t->data, t->texture, renderer);
}

