/*
  GPL
  (c) 2019, thorsten.johannvorderbrueggen@t-online.de

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

#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include "loader_params.h"

class Game_object
{
public:
	virtual void draw() = 0;
	virtual void update_all() = 0;
	virtual void cleanup_all() = 0;

protected:
	Game_object(const Loader_params* params) {}
	virtual ~Game_object() {}
};


#endif
