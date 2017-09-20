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

#define PROGNAME "use joystick or keyboard control the flying astronaut"
#define SPRITE_SHEET "astronaut.png"

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

/* show some additional infos */
bool enable_debug = false;

/* the player parts */
game_obj_data_t *player;
vector2d_t velo = {.x = 0, .y = 0};

/* all joysticks */
#define MAX_NUM_JOYSTICKS 2
SDL_Joystick *joystick_array[MAX_NUM_JOYSTICKS + 1];

/* size of window */
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

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
init_inputs(void)
{
	int err = init_joysticks(joystick_array);
	if (err == -1)
		exit(EXIT_FAILURE);

}
void
init_game_objects(void)
{
	SDL_Texture *texture = load_texture(SPRITE_SHEET, renderer);
	if (texture == NULL)
		exit(EXIT_FAILURE);

	/* player object */
	game_obj_data_t *t = alloc_game_data_object_simple(100, 100, texture);
	if (t == NULL)
		exit(EXIT_FAILURE);
	else
		player = t;
}

/*
 * cleanup all create game objects
 */
void
cleanup_game_object(void)
{
	free_game_data_object(player);
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

	draw_object(player, renderer);

	/* bring everthing to the window -> now we see the changes */
	SDL_RenderPresent(renderer);
}

/*
 *  update all needed stuff
 */
void
update_all(void)
{
	set_object_velo(player, &velo);

	int x = get_object_pos_x(player);
	if (x > (SCREEN_WIDTH - get_object_size_w(player)) || x < 0)
		inv_vec_x(&velo);

	int y = get_object_pos_y(player);
	if (y > (SCREEN_HEIGHT - get_object_size_h(player)) || y < 0)
		inv_vec_y(&velo);

	if (enable_debug)
		show_object_kine_vals(player);
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
			printf("quit the example -> bye %s\n", getenv("USER"));
			running = false;
			break;
		case SDL_JOYAXISMOTION:
			printf("SDL_JOYAXISMOTION of: %d\n", e.jaxis.which);
			tip_joystick_axis_move(&e, &velo, 1);
			break;
		case SDL_JOYBUTTONDOWN:
                        /* do something */
			break;
		case SDL_JOYBUTTONUP:
			/* do something */
			break;
		case SDL_KEYDOWN:
			printf("SDL_KEYDOWN\n");
			tip_keyboard_cursor_move(&velo, 1);
			handle_keyboard_calc_keys(&velo, 1);
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
main(int argc, char *argv[])
{
	printf("usage: ./flying astronaut [-d]                          \n");
	printf("       -d -> show actual postion and velocity           \n");
	printf("       use cursor keys and/or WASD to move the astronaut\n");
	printf("       use +/- keys to add 1 to velocity(x and y!)      \n");

	int c;
	while ((c = getopt(argc, argv, "d")) != -1) {
		switch (c) {
		case 'd':
			enable_debug = true;
			printf("show actual pos and velo\n");
			break;
		default:
			fprintf(stderr, "no valid option\n");
			exit(EXIT_FAILURE);
		}
	}

	init_game();
	init_inputs();
	init_game_objects();

        /* init done */
	running = true;

	uint32_t frame_start, frame_time;
	unsigned char local_fps =  FPS + 1; /* the first run never fits */
	uint32_t delay_time = 1000.0f / local_fps;

	while (running) {
		frame_start = SDL_GetTicks();

		HANDLE_EVENTS();
		UPDATE_ALL();
		RENDER_ALL();

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

	cleanup_game_object();
	free_joysticks(joystick_array);
	cleanup_main_window(window, renderer);

	exit(EXIT_SUCCESS);
}
