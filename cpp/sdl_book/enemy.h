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

#ifndef _ENEMY_H_
#define _ENEMY_H


#include <SDL.h>
#include <SDL_log.h>

#include "loader_params.h"
#include "sdl_game_object.h"

class Enemy : public SDLGame_object
{
public:
	Enemy(const Loader_params* params);

	virtual void draw();
	virtual void update_all();
	virtual void cleanup_all();
};

#endif
