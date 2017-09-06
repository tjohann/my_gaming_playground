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


inline LIGGAME_EXPORT void
add_vec(vector2d_t *a, vector2d_t *b)
{
	a->x += b->x;
	a->y += b->y;
}

inline LIGGAME_EXPORT void
sub_vec(vector2d_t *a, vector2d_t *b)
{
	a->x -= b->x;
	a->y -= b->y;
}

inline LIGGAME_EXPORT void
scal_mul_vec(vector2d_t *a, float b)
{
	a->x *= b;
	a->y *= b;
}

inline LIGGAME_EXPORT void
scal_div_vec(vector2d_t *a, float b)
{
	a->x /= b;
	a->y /= b;
}

inline LIGGAME_EXPORT float
lenght_vec(vector2d_t *a)
{
	return (sqrt((a->x) * (a->x) + (a->y) * (a->y)));
}

inline LIGGAME_EXPORT void
norm_vec(vector2d_t *a)
{
	float l = lenght_vec(a);
	if (l > 1) {
		a->x /= l;
		a->y /= 1;
	}
}
