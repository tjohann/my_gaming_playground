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
init_game_object(int x, int y, int w, int h, SDL_Texture *texture)
{
	game_obj_t *t = malloc(sizeof(game_obj_t));
	if (t == NULL)
		err_and_ret("could not alloc mem", NULL);

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

LIGGAME_EXPORT game_obj_t *
init_game_object_from_file(char *filename, int x, int y, int w, int h,
			SDL_Renderer *renderer)
{
	SDL_Texture *texture = load_texture(filename, renderer);
	if (texture == NULL)
		err_and_ret("could not load texture", NULL);

	/* static objects */
	game_obj_t *t = init_game_object(x, y, w, h, texture);
	if (t == NULL)
		err_and_ret("could not init game object", NULL);

	return t;
}

LIGGAME_EXPORT void
free_game_object(game_obj_t *t)
{
	if (t != NULL)
		free(t);
	else
		printf("game object == NULL\n");
}

LIGGAME_EXPORT void
draw_object(game_obj_t *obj, SDL_Renderer *renderer)
{
	if (obj->frame == -1)
		draw_texture(obj, renderer);
	else
		draw_frame_texture(obj, renderer);
}

LIGGAME_EXPORT void
set_object_frame(game_obj_t *obj, signed char frame)
{
	obj->frame = frame;
}
