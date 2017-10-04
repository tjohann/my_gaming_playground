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
alloc_game_object_simple(char *name, int x, int y, SDL_Texture *texture)
{
	game_obj_t *t = malloc(sizeof(game_obj_t));
	if (t == NULL)
		goto error;
	memset(t, 0, sizeof(game_obj_t));

	t->data = alloc_game_data_object_simple(x, y, texture);
	if (t == NULL)
		goto error;

	t->func = alloc_game_func_object_simple();
	if (t == NULL)
		goto error;

	t->name = alloc_string(name);
	if (t == NULL)
		goto error;

	return t;
error:
	free_game_object(t);
	err_and_ret("could not alloc mem", NULL);
}

LIGGAME_EXPORT game_obj_t *
alloc_game_object_from_array(char *name, char *texture_name, int x, int y,
			game_texture_t a[])
{
	for (int i = 0; a[i].name != NULL; i++) {
		if (strlen(texture_name) != strlen(a[i].name))
			continue;
		if (strncmp(a[i].name, texture_name, strlen(texture_name)) != 0)
			continue;

		printf("found a[%d].name: %s\n", i, a[i].name);
		return alloc_game_object_simple(name, x, y, a[i].texture);
	}

	eprintf("no texture for for %s found\n", name);
	return NULL;
}

LIGGAME_EXPORT void
free_game_object(game_obj_t *obj)
{
	if (obj != NULL) {
		if (obj->data->texture != NULL)
			SDL_DestroyTexture(obj->data->texture);
		free_game_data_object(obj->data);
		free_game_func_object(obj->func);
		free(obj->name);
		free(obj);
	} else
		printf("game object == NULL\n");
}

LIGGAME_EXPORT game_obj_data_t *
alloc_game_data_object(int x, int y, int w, int h, SDL_Texture *texture)
{
	game_obj_data_t *t = malloc(sizeof(game_obj_data_t));
	if (t == NULL)
		err_and_ret("could not alloc mem", NULL);

	memset(t, 0, sizeof(game_obj_data_t));

	t->pos.x = x;
	t->pos.y = y;
	t->size.h = h;
	t->size.w = w;

	/*
	 * frame == -1 -> no sprite sheet
	 * frame != -1 -> frame position within the sprite sheet
	 */
	t->frame = -1;

	t->texture = texture;
	t->flip = SDL_FLIP_NONE;

	return t;
}

LIGGAME_EXPORT game_obj_data_t *
alloc_game_data_object_simple(int x, int y, SDL_Texture *texture)
{
	int w, h;
	int err = SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	if (err < 0)
		err_and_ret("query texture", NULL);

	return alloc_game_data_object(x, y, w, h, texture);
}

LIGGAME_EXPORT game_obj_data_t *
alloc_game_data_object_from_file(char *filename, int x, int y, int w, int h,
				SDL_Renderer *renderer)
{
	SDL_Texture *texture = load_texture(filename, renderer);
	if (texture == NULL)
		err_and_ret("could not load texture", NULL);

	game_obj_data_t *t = alloc_game_data_object(x, y, w, h, texture);
	if (t == NULL)
		err_and_ret("could not init game object", NULL);

	return t;
}

LIGGAME_EXPORT game_obj_data_t *
alloc_game_data_object_from_file_simple(char *filename, int x, int y,
				SDL_Renderer *renderer)
{
	SDL_Texture *texture = load_texture(filename, renderer);
	if (texture == NULL)
		err_and_ret("could not load texture", NULL);

	game_obj_data_t *t = alloc_game_data_object_simple(x, y, texture);
	if (t == NULL)
		err_and_ret("could not init game object", NULL);

	return t;
}

LIGGAME_EXPORT void
free_game_data_object(game_obj_data_t *obj)
{
	if (obj != NULL)
		free(obj);
	else
		printf("game data object == NULL\n");
}

LIGGAME_EXPORT game_obj_func_t *
alloc_game_func_object_simple(void)
{
	game_obj_func_t *t = malloc(sizeof(game_obj_func_t));
	if (t == NULL)
		err_and_ret("could not alloc mem", NULL);

	memset(t, 0, sizeof(game_obj_func_t));

	/* set the defaults */
	t->draw = draw_object;
	t->update = set_object_velo;
	t->collision_window = collision_window;
	t->collision_object = collision_object;
	t->detect_collision_object = detect_collision_object;

	return t;
}

LIGGAME_EXPORT void
free_game_func_object(game_obj_func_t *obj)
{
	if (obj != NULL)
		free(obj);
	else
		printf("game func object == NULL\n");
}

LIGGAME_EXPORT void
draw_object(game_obj_data_t *obj, SDL_Renderer *renderer)
{
	if (obj->frame == -1)
		draw_texture(obj, renderer);
	else
		draw_frame_texture(obj, renderer);
}

LIGGAME_EXPORT void
set_object_frame(game_obj_data_t *obj, signed char frame)
{
	obj->frame = frame;
}

LIGGAME_EXPORT void
set_object_velo(game_obj_data_t *obj, vector2d_t *velo)
{
	obj->velo.x = velo->x;
	obj->velo.y = velo->y;

	/* set new pos */
	add_vec(&obj->pos, &obj->velo);
}

LIGGAME_EXPORT int
get_object_velo_x(game_obj_data_t *obj)
{
	return obj->velo.x;
}

LIGGAME_EXPORT int
get_object_velo_y(game_obj_data_t *obj)
{
	return obj->velo.y;
}

LIGGAME_EXPORT vector2d_t *
get_object_velo(game_obj_data_t *obj)
{
	return &obj->velo;
}

LIGGAME_EXPORT int
get_object_pos_x(game_obj_data_t *obj)
{
	return obj->pos.x;
}

LIGGAME_EXPORT int
get_object_pos_y(game_obj_data_t *obj)
{
	return obj->pos.y;
}

LIGGAME_EXPORT vector2d_t *
get_object_pos(game_obj_data_t *obj)
{
	return &obj->pos;
}

LIGGAME_EXPORT void
clear_object_pos_x(game_obj_data_t *obj)
{
	obj->pos.x = 0;
}

LIGGAME_EXPORT void
clear_object_pos_y(game_obj_data_t *obj)
{
	obj->pos.y = 0;
}

LIGGAME_EXPORT void
clear_object_pos(game_obj_data_t *obj)
{
	obj->pos.x = 0;
	obj->pos.y = 0;
}

LIGGAME_EXPORT void
set_object_pos_x(game_obj_data_t *obj, int x)
{
	obj->pos.x = x;
}

LIGGAME_EXPORT void
set_object_pos_y(game_obj_data_t *obj, int y)
{
	obj->pos.y = y;
}

LIGGAME_EXPORT void
set_object_pos_x_y(game_obj_data_t *obj, int x, int y)
{
	obj->pos.x = x;
	obj->pos.y = y;
}

LIGGAME_EXPORT void
set_object_pos(game_obj_data_t *obj, vector2d_t *pos)
{
	obj->pos.x = pos->x;
	obj->pos.y = pos->y;
}

LIGGAME_EXPORT void
set_object_pos_via_mouse(game_obj_data_t *obj, SDL_Event *e, int frac)
{
	vector2d_t mouse_pos;
	get_mouse_position(e, &mouse_pos);

	obj->pos.x = mouse_pos.x / frac;
	obj->pos.y = mouse_pos.y / frac;
}

LIGGAME_EXPORT int
get_object_size_w(game_obj_data_t *obj)
{
	return obj->size.w;
}

LIGGAME_EXPORT int
get_object_size_h(game_obj_data_t *obj)
{
	return obj->size.h;
}

LIGGAME_EXPORT spread_t *
get_object_size(game_obj_data_t *obj)
{
	return &obj->size;
}

LIGGAME_EXPORT void
calc_object_surface_pos(game_obj_data_t *obj, int *l, int *r, int *t, int *b)
{
	*l = obj->pos.x;
	*r = obj->pos.x + obj->size.w;
	*t = obj->pos.y;
	*b = obj->pos.y + obj->size.h;
}

