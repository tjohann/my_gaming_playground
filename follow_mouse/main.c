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

#define PROGNAME "use mouse to move a game object"
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

/* the player parts */
game_obj_data_t *player;
vector2d_t velo = {.x = 0, .y = 0};

/* size of window */
const uint32_t SCREEN_WIDTH = 1280;
const uint32_t SCREEN_HEIGHT = 720;

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
	SDL_Texture *texture = load_texture(SPRITE_SHEET, renderer);
	if (texture == NULL)
		exit(EXIT_FAILURE);

	/* static object */
	game_obj_data_t *t = alloc_game_data_object_simple(1, 1, texture);
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
	uint32_t x = get_object_pos_x(player);
	if (x > SCREEN_WIDTH)
		set_object_pos_x(player, 0);
	else if (x == 0)
		set_object_pos_x(player, SCREEN_WIDTH);

	uint32_t y = get_object_pos_y(player);
	if (y > SCREEN_HEIGHT)
		set_object_pos_y(player, 0);
	else if (y == 0)
		set_object_pos_y(player, SCREEN_HEIGHT);

	set_object_velo(player, &velo);
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
		case SDL_MOUSEBUTTONDOWN:
			printf("MOUSEBUTTONDOWN");

			if (e.button.button == SDL_BUTTON_LEFT) {
				printf("left botton\n");
				set_object_pos_via_mouse(player, &e, 2);
			}

			if (e.button.button == SDL_BUTTON_MIDDLE)
				printf("middle botton\n");

			if (e.button.button == SDL_BUTTON_RIGHT)
				printf("right botton\n");

			break;
		case SDL_MOUSEBUTTONUP:
			printf("MOUSEBUTTONUP\n");
			clear_vec(&velo);
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
	unsigned char local_fps = FPS + 1; /* the first run never fits */
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
	cleanup_main_window(window, renderer);

	exit(EXIT_SUCCESS);
}
