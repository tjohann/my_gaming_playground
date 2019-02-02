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

#include "game.h"

#include <iostream>

int
main(int argc, char *argv[])
{
	if (the_game::instance()->init("sdl_game", 100, 100, 800, 600, false))
	{
		std::cerr << "could not init game" << std::endl;
		exit(EXIT_FAILURE);
	}

	while(the_game::instance()->is_running()) {
		the_game::instance()->handle_events();
		the_game::instance()->update_all();
		the_game::instance()->render_all();

		SDL_Delay(10);
	}

	the_game::instance()->cleanup_all();

	exit(EXIT_SUCCESS);
}
