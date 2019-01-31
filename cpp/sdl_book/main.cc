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

Game *game = NULL;

int
main(int argc, char *argv[])
{
	game = new Game();

	/* for testing */
	game->init("sdl_game", 100, 100, 800, 600, false);
	//game->init("sdl_game", 0, 0, 800, 600, false);
	//game->init("sdl_game", 100, 100, 800, 600, true);

	while(game->is_running()) {
		game->handle_events();
		game->update_all();
		game->render_all();

	}

	game->cleanup_all();

	exit(EXIT_SUCCESS);
}
