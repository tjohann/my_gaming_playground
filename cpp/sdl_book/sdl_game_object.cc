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

#include "sdl_game_object.h"
#include "loader_params.h"
#include "texture_manager.h"
#include "game.h"

SDLGame_object::SDLGame_object(const Loader_params* params)
	: Game_object(params)
{
	x_ = params->get_x();
	y_ = params->get_y();

	w_ = params->get_w();
	h_ = params->get_h();

	texture_id_ = params->get_texture_id();

	current_row_ = 1;
	current_frame_ = 1;

}

void SDLGame_object::draw()
{
	the_texture_manager::instance()->draw_frame("animate",
						    0, 0,
						    128, 82,
						    current_row_, current_frame_,
						    the_game::instance()->get_renderer(),
						    SDL_FLIP_NONE);

}
