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


LIGGAME_EXPORT void
collision_window(game_obj_data_t *obj, vector2d_t *velo,
		const int w, const int h)
{
	int x = get_object_pos_x(obj);
	if (x > (w - get_object_size_w(obj)) || x < 0)
		inv_vec_x(velo);

	int y = get_object_pos_y(obj);
	if (y > (h - get_object_size_h(obj)) || y < 0)
		inv_vec_y(velo);
}

LIGGAME_EXPORT void
collision_object(game_obj_data_t *a, game_obj_data_t *b, vector2d_t *velo)
{
	int a_left   = 0;
	int a_right  = 0;
	int a_top    = 0;
	int a_bottom = 0;
	calc_object_surface_pos(a, &a_left, &a_right, &a_top, &a_bottom);

	int b_left   = 0;
	int b_right  = 0;
	int b_top    = 0;
	int b_bottom = 0;
	calc_object_surface_pos(b, &b_left, &b_right, &b_top, &b_bottom);

	if (a_bottom <= b_top)
		return;

	if (a_top >= b_bottom)
		return;

	if (a_right <= b_left)
		return;

	if (a_left >= b_right)
		return;

	if ((a_right >= b_left) && (a_right < b_right)) {
		printf("left\n");
		inv_vec_x(velo);
	}

	if ((a_left <= b_right) && (a_left > b_left)) {
		printf("right\n");
		inv_vec_x(velo);
	}

	if ((a_bottom >= b_top) && (a_bottom < b_bottom)) {
		printf("top\n");
		inv_vec_y(velo);
	}

	if ((a_top <= b_bottom) && (a_top > b_top)) {
		printf("bottom\n");
		inv_vec_y(velo);
	}
}

LIGGAME_EXPORT bool
detect_collision_object(game_obj_data_t *a, game_obj_data_t *b)
{
	int a_left   = 0;
	int a_right  = 0;
	int a_top    = 0;
	int a_bottom = 0;
	calc_object_surface_pos(a, &a_left, &a_right, &a_top, &a_bottom);

	int b_left   = 0;
	int b_right  = 0;
	int b_top    = 0;
	int b_bottom = 0;
	calc_object_surface_pos(b, &b_left, &b_right, &b_top, &b_bottom);

	if (a_bottom <= b_top)
		return false;

	if (a_top >= b_bottom)
		return false;

	if (a_right <= b_left)
		return false;

	if (a_left >= b_right)
		return false;

	return true;
}
