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

#define PROGNAME "use use_game_obj_t for game objects"
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


/* main window and renderer*/
SDL_Window   *window;
SDL_Renderer *renderer;

/* the global state -> true still running, false quit */
bool running = false;

/* all game objects */
#define MAX_NUM_OBJ 10
game_obj_t *static_obj_array[MAX_NUM_OBJ];
game_obj_t *moving_obj_array[MAX_NUM_OBJ];


/*
 * do all init stuff
 */
void
init_game(void)
{
	window = setup_main_window(PROGNAME, 1024, 768, 0);
	if (window == NULL)
		exit(EXIT_FAILURE);

	renderer = setup_renderer(window, NULL);
	if (renderer == NULL)
		exit(EXIT_FAILURE);
}

void
init_game_objects(void)
{
	SDL_Texture *texture = load_texture(SPRITE_SHEET, renderer);
	if (texture == NULL)
		exit(EXIT_FAILURE);

	/* static objects */
	game_obj_t *t = init_game_object(0, 0, 128, 82, texture);
	if (t == NULL)
		exit(EXIT_FAILURE);
	else
		static_obj_array[0] = t;

	t = init_game_object(100, 100, 128, 82, texture);
	if (t == NULL)
		exit(EXIT_FAILURE);
	else
		static_obj_array[1] = t;

	/* flip this object */
	static_obj_array[1]->flip = SDL_FLIP_HORIZONTAL;

	static_obj_array[2] = NULL;

	/* moving objects */
	t = init_game_object(200, 200, 128, 82, texture);
	if (t == NULL)
		exit(EXIT_FAILURE);
	else
		moving_obj_array[0] = t;

	moving_obj_array[1] = NULL;
}

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

	for(int i = 0; i < MAX_NUM_OBJ; i++) {
		if (static_obj_array[i] != NULL)
			draw_texture(static_obj_array[i]->texture, renderer,
				static_obj_array[i]->pos, static_obj_array[i]->flip);
		else
			break;
	}

	for(int i = 0; i < MAX_NUM_OBJ; i++) {
		if (moving_obj_array[i] != NULL)
			draw_frame_texture(moving_obj_array[i]->texture, renderer,
					moving_obj_array[i]->pos, moving_obj_array[i]->frame,
					moving_obj_array[i]->flip);
		else
			break;
	}

	/* bring everthing to the window -> now we see the changes */
	SDL_RenderPresent(renderer);
}

/*
 *  update all needed stuff
 */
void
update_all(void)
{
	unsigned char act_frame = (SDL_GetTicks() / 100) % 6;

	for(int i = 0; i < MAX_NUM_OBJ; i++) {
		if (moving_obj_array[i] != NULL)
			moving_obj_array[i]->frame = act_frame;
		else
			break;
	}
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
	init_game();
	init_game_objects();

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
