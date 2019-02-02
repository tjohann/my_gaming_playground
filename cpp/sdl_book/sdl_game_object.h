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

#ifndef _SDL_GAME_OBJECT_H_
#define _SDL_GAME_OBJECT_H

#include <vector>

#include <SDL.h>
#include <SDL_log.h>

#include "loader_params.h"
#include "game_object.h"

class SDLGame_object : public Game_object
{
public:
	SDLGame_object(const Loader_params* params);

	virtual void draw();
	virtual void update_all();
	virtual void cleanup_all();

protected:
	int x_;
	int y_;

	int w_;
	int h_;

	int current_row_;
	int current_frame_;

	std::string texture_id_;
};

#endif
