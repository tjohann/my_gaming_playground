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

#define RENDER_ALL() do {			\
		render_window();		\
	} while (0)

#define HANDLE_EVENTS() do {			\
		handle_events();		\
	} while (0)

#define UPDATE_ALL() do {			\
		update_all();			\
	} while (0)


/* to make the code more clear */
#define HANDLE_JOYSTICKS() do {						\
		unsigned char which = e.jaxis.which;			\
		joystick_array[which].handle_axis_joystick(&e,		\
							joystick_array[which].to_change, \
							joystick_array[which].step); \
	} while (0)

#define HANDLE_QUIT() do {						\
		printf("quit the example -> bye %s\n", getenv("USER")); \
		running = false;					\
	} while(0)


/* the global state -> true still running, false quit */
bool running = false;
bool gaming = false;

char *progname = "simple_tennis";
char *config_file = "simple_tennis.conf";

SDL_Window   *window;
SDL_Renderer *renderer;

spread_t screen;

game_texture_t *texture_array;
game_joystick_t *joystick_array;

game_obj_t **players;
game_obj_t **balls;


/*
 * do all init stuff
 */
void
init_game(void)
{
	config_t cfg;
	int err = open_config(config_file, progname, &cfg);
	if (err == -1)
		exit(EXIT_FAILURE);

	/* setup main window */
	window = setup_main_window_via_config(&cfg, 0, &screen);
	if (window == NULL)
		exit(EXIT_FAILURE);

	renderer = setup_renderer_via_config(&cfg, window);
	if (renderer == NULL)
		exit(EXIT_FAILURE);

	/* load all textures */
	texture_array = alloc_textures_via_config(&cfg, renderer);
	if (texture_array == NULL)
		exit(EXIT_FAILURE);

	/* alloc the two player objects */
	players = alloc_player_objects_via_config(&cfg, texture_array);
	if (players == NULL)
		exit(EXIT_FAILURE);

	for (int i = 0; players[i] != NULL; i++)
		printf("name of player %s\n", players[i]->name);

	/* alloc the ball objects */
	balls = alloc_static_objects_via_config(&cfg, texture_array);
	if (balls == NULL)
		exit(EXIT_FAILURE);

	for (int i = 0; balls[i] != NULL; i++)
		printf("name of static object %s\n", balls[i]->name);

	joystick_array = alloc_joystick_objects_via_config(&cfg);
	if (joystick_array == NULL)
		exit(EXIT_FAILURE);

	for (int i = 0; joystick_array[i].name != NULL; i++)
		printf("name of joystick %s\n", joystick_array[i].name);

	config_destroy(&cfg);

	connect_player_objects(joystick_array, players);
}

/*
 * set the position of all objects depending of th screen size
 */
void
set_position_of_objects(void)
{
	int space_x = screen.w * 15 / 100;
	int space_y = get_object_size_h(players[1]->data) / 2;

	/* the left player */
	vector2d_t tmp = { .x = space_x, .y = (screen.h / 2) - space_y};
	set_object_pos(players[0]->data, &tmp);

	/* the right player */
	space_x += get_object_size_w(players[1]->data);
	tmp.x = screen.w - space_x;
	tmp.y = (screen.h / 2) - space_y;
	set_object_pos(players[1]->data, &tmp);

	/* the ball */
	space_x = space_y = get_object_size_w(balls[0]->data);
	tmp.x = (screen.w / 2) - space_x;
	tmp.y = (screen.h / 2) - space_y;
	set_object_pos(balls[0]->data, &tmp);
}

/*
 * cleanup all create objects
 */
void
cleanup_game(void)
{
	for (int i = 0; players[i] != NULL; i++)
		free_game_object(players[i]);

	for (int i = 0; balls[i] != NULL; i++)
		free_game_object(balls[i]);

	free_texture_array(texture_array);
	free_joystick_array(joystick_array);

	cleanup_main_window(window, renderer);
}

/*
 * render all parts
 */
void
render_window(void)
{
	int err = SDL_RenderClear(renderer);
	if (err < 0)
		fprintf(stderr, "could not set clear rendering target (%s)\n",
			SDL_GetError());

	for (int i = 0; players[i] != NULL; i++)
		players[i]->func->draw(players[i]->data, renderer);

	for (int i = 0; balls[i] != NULL; i++)
		balls[i]->func->draw(balls[i]->data, renderer);

	SDL_RenderPresent(renderer);
}

/*
 * start the ball via space
 */
void
tip_start(vector2d_t *mov_vec)
{
	const uint8_t *k = SDL_GetKeyboardState(NULL);

	if (k[SDL_SCANCODE_SPACE]) {

		if (gaming) {
			gaming = false;
		} else {
			mov_vec->x = get_random_value();
			mov_vec->y = get_random_value();

			gaming = true;
		}
	}
}


/*
 *  update all needed stuff
 */
void
update_all(void)
{
	for (int i = 0; players[i] != NULL; i++) {
		players[i]->func->update(players[i]->data,
					&players[i]->new_velo);

		players[i]->func->collision_window(players[i]->data,
						&players[i]->new_velo, &screen);

		for (int j = 0; balls[j] != NULL; j++)
			players[i]->func->collision_object(players[i]->data,
							balls[j]->data,
							&players[i]->new_velo);
	}

	for (int i = 0; balls[i] != NULL; i++) {
		balls[i]->func->update(balls[i]->data, &balls[i]->new_velo);

		balls[i]->func->collision_window(balls[i]->data,
						&balls[i]->new_velo, &screen);
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
			HANDLE_QUIT();
			break;
		case SDL_JOYAXISMOTION:
			printf("SDL_JOYAXISMOTION\n");
			HANDLE_JOYSTICKS();
			break;
		case SDL_JOYBUTTONDOWN:
                        /* do something */
			break;
		case SDL_JOYBUTTONUP:
			/* do something */
			break;
		case SDL_KEYDOWN:
			for (int i = 0; balls[i] != NULL; i++)
				tip_start(&balls[i]->new_velo);
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
	printf("usage: ./simple tennis                               \n");
	printf("       something like a pong clone                   \n");

	atexit(cleanup_game);

	init_game();
	set_position_of_objects();

        /* init done */
	running = true;

	uint32_t frame_start, frame_time;
	unsigned char local_fps =  FPS + 1; /* the first run never fits */
	uint32_t delay_time = 1000.0f / local_fps;

	while (running) {
		frame_start = SDL_GetTicks();

		HANDLE_EVENTS();

		if (gaming) {
			UPDATE_ALL();
		} else {
			set_position_of_objects();
			printf("not im gaming mode\n");
		}

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

	exit(EXIT_SUCCESS);
}
