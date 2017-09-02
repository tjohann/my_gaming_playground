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

#include <libgame.h>

#define PROGNAME "example for using libgame"
#define SPRITE_SHEET "animate-alpha.png"

#define RENDER_ALL() do {			\
		/* do something */		\
	} while (0)

#define HANDLE_EVENTS() do {			\
		handle_events();		\
	} while (0)

#define UPDATE_ALL() do {			\
		/* do something */		\
	} while (0)


/* the global running variable -> true still running, false quit */
bool running = false;


/*
 *  handle supported events
 */
void
handle_events()
{
	SDL_Event e;
	if (SDL_PollEvent(&e)) {
		switch(e.type) {
		case SDL_QUIT:
			printf("an actual SDL_QUIT event occured\n");
			running = false;
			break;
		default:
			printf("an actual unsupported event occured %d\n",
				e.type);
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
	SDL_Window   *window   = NULL;
	SDL_Renderer *renderer = NULL;

	window = setup_main_window(PROGNAME, 1024, 768, 0);
	if (window == NULL)
		exit(EXIT_FAILURE);

	renderer = setup_renderer(window, NULL);
	if (renderer == NULL)
		exit(EXIT_FAILURE);





        /* init done */
	running = true;

	while (running) {
		HANDLE_EVENTS();
		UPDATE_ALL();
		RENDER_ALL();
	}

	cleanup_main_window(window, renderer);

	exit(EXIT_SUCCESS);
}
