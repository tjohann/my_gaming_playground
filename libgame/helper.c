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
show_object_kine_vals(game_obj_t *obj)
{
	printf("-----------------------------------\n");
	printf("position x/y %d/%d\n", obj->pos.x, obj->pos.y);
	printf("veloctiy x/y %d/%d\n", obj->velo.x,  obj->velo.y);
	printf("-----------------------------------\n\n");
}
