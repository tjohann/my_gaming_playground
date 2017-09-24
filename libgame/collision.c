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
	int a_l = 0;    /* left   */
	int a_r = 0;    /* right  */
	int a_t = 0;    /* top    */
	int a_b = 0;    /* bottom */
	calc_object_surface_pos(a, &a_l, &a_r, &a_t, &a_b);

	int b_l = 0;
	int b_r = 0;
	int b_t = 0;
	int b_b = 0;
	calc_object_surface_pos(b, &b_l, &b_r, &b_t, &b_b);

	/*
	 * check for collision
	 */

	if (a_b <= b_t)
		return;

	if (a_t >= b_b)
		return;

	if (a_r <= b_l)
		return;

	if (a_l >= b_r)
		return;

	/*
	 * check direction:
	 *
	 * - a moves
	 * - b standing still
	 *
	 *            +----+
	 *            |    |
	 *    +----+  | -b |
	 *    |    |  +----+
	 *    | -a |
	 *    +----+
	 *
	 * - due to velocity steps the sides of -a will collide on two sides
	 *   of -b:
	 *
	 *        +----+
	 *        |    |
	 *    +---|+ b |
	 *    |   +|---+
	 *    | -a |
	 *    +----+
	 *
	 * - this leads to detect a collision of left and bottom
	 * - to distinguish between left and bottom, calculate the overlapping
	 *   of a right and b left and a top and b bottom
	 * - the large one should be to direction
	 */
	bool l_ = false, r_ = false, t_ = false, b_ = false;
	if ((a_r >= b_l) && (a_r < b_r))
		l_ = true;

	if ((a_l <= b_r) && (a_l > b_l))
		r_ = true;

	if ((a_b >= b_t) && (a_b < b_b))
		t_ = true;

	if ((a_t <= b_b) && (a_t > b_t))
		b_ = true;

	if (l_ && b_) {
		if ((b_b - a_t) > (a_r - b_l))
			inv_vec_x(velo);
		else
			inv_vec_y(velo);
		return;
	}

	if (l_ && t_) {
		if ((a_b - b_t) > (a_r - b_l))
			inv_vec_x(velo);
		else
			inv_vec_y(velo);
		return;
	}

	if (r_ && b_) {
		if ((b_b - a_t) > (b_r - a_l))
			inv_vec_x(velo);
		else
			inv_vec_y(velo);
		return;
	}

	if (r_ && t_) {
		if ((a_b - b_t) > (b_r - a_l))
			inv_vec_x(velo);
		else
			inv_vec_y(velo);
		return;
	}

	if (l_ || r_)
		inv_vec_x(velo);

	if (b_ || t_)
		inv_vec_y(velo);
}

LIGGAME_EXPORT bool
detect_collision_object(game_obj_data_t *a, game_obj_data_t *b)
{
	int a_l = 0;    /* left   */
	int a_r = 0;    /* right  */
	int a_t = 0;    /* top    */
	int a_b = 0;    /* bottom */
	calc_object_surface_pos(a, &a_l, &a_r, &a_t, &a_b);

	int b_l = 0;
	int b_r = 0;
	int b_t = 0;
	int b_b = 0;
	calc_object_surface_pos(b, &b_l, &b_r, &b_t, &b_b);

	if (a_b <= b_t)
		return false;

	if (a_t >= b_b)
		return false;

	if (a_r <= b_l)
		return false;

	if (a_l >= b_r)
		return false;

	return true;
}
