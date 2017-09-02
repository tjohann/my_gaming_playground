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
		render_window();		\
	} while (0)

#define HANDLE_EVENTS() do {			\
		handle_events();		\
	} while (0)

#define UPDATE_ALL() do {			\
		update_all();			\
	} while (0)


/* global parts */
SDL_Window   *window;
SDL_Renderer *renderer;
SDL_Texture  *texture;

unsigned char act_frame = 0;

/* the global running variable -> true still running, false quit */
bool running = false;

/*
 * render all parts
 */
void
render_window(void)
{
        /* clear rendering target to the drawing color */
	int err = SDL_RenderClear(renderer);
	if (err < 0)
		fprintf(stderr, "could not set clear rendering target (%s)\n",
			SDL_GetError());

	/* first */
	pos_t pos = { .x = 0, .y = 0, .w = 128, .h = 82};
	draw_texture(texture, renderer, pos, SDL_FLIP_NONE);

	/* second */
	pos_t pos_2 = { .x = 100, .y = 100, .w = 128, .h = 82};
	draw_frame_texture(texture, renderer, pos_2, act_frame, SDL_FLIP_NONE);

	/* third */
	pos_t pos_3 = { .x = 200, .y = 200, .w = 128, .h = 82};
	draw_frame_texture(texture, renderer, pos_3, act_frame, SDL_FLIP_HORIZONTAL);

	/* bring everthing to the window -> now we see the changes */
	SDL_RenderPresent(renderer);
}

/*
 *  update all needed stuff
 */
void
update_all(void)
{
	act_frame = (SDL_GetTicks() / 100) % 6;
}

/*
 *  handle supported events
 */
void
handle_events(void)
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
	window = setup_main_window(PROGNAME, 1024, 768, 0);
	if (window == NULL)
		exit(EXIT_FAILURE);

	renderer = setup_renderer(window, NULL);
	if (renderer == NULL)
		exit(EXIT_FAILURE);

	texture = load_texture(SPRITE_SHEET, renderer);
	if (texture == NULL)
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
