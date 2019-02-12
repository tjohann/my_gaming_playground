/*
  GPL
  (c) 2017-2019, thorsten.johannvorderbrueggen@t-online.de

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


LIGGAME_EXPORT int
init_game(game_t *game)
{
	config_t cfg;
	int err = open_config(game->config, game->name, &cfg);
	if (err == -1)
		exit(EXIT_FAILURE);

        /* setup main window */
	err = setup_window_via_config(&cfg, game);
	if (err == -1)
		goto error;

	err = setup_renderer_via_config(&cfg, game);
	if (err == -1)
		goto error;

	/* load all textures */
	err = alloc_textures_via_config(&cfg, game);
	if (err == -1)
		goto error;


	/*
	 * TODO: NEXT -> alloc_objects_via_config
	 */

	return 0;
error:
	config_destroy(&cfg);

	err_and_ret("could not init game", -1);
}

LIGGAME_EXPORT void
cleanup_game(game_t *game)
{
	/*
	 * TODO: cleanup the different elements
	 *
	 * - textures
	 */

	cleanup_window(game->window, game->renderer);
}
