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

#include <libgame.h>

/* the global state -> true still running, false quit */
bool running = false;


game_t game = {
	.name = "moving_tiled",
	.config = "moving_tiled.conf"
};


void
cleanup_all(void)
{
	cleanup_game(&game);
}

void
render_all(void)
{

	int err = SDL_RenderClear(game.renderer);
	if (err < 0)
		fprintf(stderr, "could not set clear rendering target (%s)\n",
			SDL_GetError());



	SDL_RenderPresent(game.renderer);
}

void
update_all(void)
{
        // printf("%s\n", __FUNCTION__);
}

void
handle_events(void)
{
	SDL_Event e;
	if (SDL_PollEvent(&e)) {
		switch(e.type) {
		case SDL_QUIT:
			printf("you want to quit \"%s\" -> bye %s\n",
				game.name, getenv("USER"));
			running = false;
			break;
		case SDL_JOYAXISMOTION:
			printf("SDL_JOYAXISMOTION\n");
			/* do something */
			break;
		case SDL_JOYBUTTONDOWN:
                        /* do something */
			break;
		case SDL_JOYBUTTONUP:
			/* do something */
			break;
		case SDL_KEYDOWN:
			/* do something */
			break;
		case SDL_KEYUP:
			/* do something */
			break;
		case SDL_MOUSEBUTTONDOWN:
			/* do something */
			break;
		case SDL_MOUSEBUTTONUP:
			/* do something */
			break;
		default:
			/* ignore this time */
			break;
		}
	}
}


/*
 * -----------------------------------------------------------------------------
 */
int
main(void)
{
	printf("usage: ./%s \n", game.name);
	printf("       simple demo which renders a tiled set         \n");
	printf("       use a joystick or keyboard to \"move\" around \n");

	if (init_game(&game) == -1)
		exit(EXIT_FAILURE);

	atexit(cleanup_all);

        /* init done */
	running = true;

	uint32_t frame_start, frame_time;
	unsigned char local_fps =  FPS + 1;
	uint32_t delay_time = 1000.0f / local_fps;

	while (running) {
		frame_start = SDL_GetTicks();

		handle_events();
		update_all();
		render_all();

		frame_time = SDL_GetTicks() - frame_start;

		if (frame_time < delay_time) {
			SDL_Delay(delay_time - frame_time);
		} else {
			printf("underrun -> try to reduce actual FPS %d\n",
				local_fps);
			local_fps--;
			delay_time = 1000.0f / local_fps;
		}
	}

	exit(EXIT_SUCCESS);
}
