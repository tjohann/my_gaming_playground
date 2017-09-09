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

#define PROGNAME "use game_obj_t for game objects"
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
game_obj_t *static_obj_array[MAX_NUM_OBJ + 1];
game_obj_t *moving_obj_array[MAX_NUM_OBJ + 1];

/* size of window */
const uint32_t SCREEN_WIDTH = 1024;
const uint32_t SCREEN_HEIGHT = 768;

/*
 * do all init stuff
 */
void
init_game(void)
{
	window = setup_main_window(PROGNAME, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (window == NULL)
		exit(EXIT_FAILURE);

	color_t background;
	background.r = 100;
	background.g = 100;
	background.b = 100;
	background.a = 255;

	renderer = setup_renderer(window, &background);
	if (renderer == NULL)
		exit(EXIT_FAILURE);
}

void
init_game_objects(void)
{
	memset(static_obj_array, 0, MAX_NUM_OBJ);
	memset(moving_obj_array, 0, MAX_NUM_OBJ);

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
 * cleanup all create game objects
 */
void
cleanup_game_object(void)
{
	int i = 0;
	while (static_obj_array[i])
		free_game_object(static_obj_array[i++]);

	i = 0;
	while (moving_obj_array[i])
		free_game_object(moving_obj_array[i++]);
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

	int i = 0;
	while (static_obj_array[i])
		draw_object(static_obj_array[i++], renderer);

	i = 0;
	while (moving_obj_array[i])
		draw_object(moving_obj_array[i++], renderer);

	/* bring everthing to the window -> now we see the changes */
	SDL_RenderPresent(renderer);
}

/*
 *  update all needed stuff
 */
void
update_all(void)
{
	/* static cat top left */
	if (get_object_pos_x(static_obj_array[0]) > SCREEN_WIDTH)
		clear_object_pos_x(static_obj_array[0]);

	vector2d_t accel = {.x = 1, .y = 0};
	set_object_accel(static_obj_array[0], &accel);


        /* static cat in the middle */
	if (get_object_pos_x(static_obj_array[1]) == 0)
		set_object_pos_x(static_obj_array[1],SCREEN_WIDTH);

	vector2d_t accel_2 = {.x = -1, .y = 0};
	set_object_accel(static_obj_array[1], &accel_2);


	/* running cat stuff */
	unsigned char act_frame = (SDL_GetTicks() / 100) % 6;

	int i = 0;
	while (moving_obj_array[i])
		set_object_frame(moving_obj_array[i++], act_frame);
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

	uint32_t frame_start, frame_time;
	uint32_t delay_time = 1000.0f / FPS;
	while (running) {
		frame_start = SDL_GetTicks();

		HANDLE_EVENTS();
		UPDATE_ALL();
		RENDER_ALL();

		frame_time = SDL_GetTicks() - frame_start;

		if (frame_time < delay_time)
			SDL_Delay(delay_time - frame_time);
		else
			printf("underrun -> try to reduce FPS?\n");
	}

	cleanup_game_object();
	cleanup_main_window(window, renderer);

	exit(EXIT_SUCCESS);
}
