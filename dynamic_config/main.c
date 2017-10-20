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


/* the global state -> true still running, false quit */
bool running = false;

char *progname = "dynamic_config";
char *config_file = "dynamic_config.conf";

game_obj_t *player;                 /* the player parts   */
game_obj_t *static_obj_array[3];    /* the fixed objects  */
game_texture_t *texture_array;      /* all textures       */

SDL_Window   *window;
SDL_Renderer *renderer;

spread_t screen;

/* all joysticks */
#define MAX_NUM_JOYSTICKS 1
SDL_Joystick *joystick_array[MAX_NUM_JOYSTICKS + 1];


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

	/* player object */
	player = alloc_game_object_from_array("player", "player", 0, 100, texture_array);
	if (player == NULL)
		exit(EXIT_FAILURE);

	/* fixed_01 object */
	static_obj_array[0] = alloc_game_object_from_array("object 1", "others", 0, 400, texture_array);
	if (static_obj_array[0] == NULL)
		exit(EXIT_FAILURE);

	/* fixed_02 object */
	static_obj_array[1] = alloc_game_object_from_array("object 2", "others", 50, 100, texture_array);
	if (static_obj_array[1] == NULL)
		exit(EXIT_FAILURE);

	static_obj_array[2] = NULL;

	config_destroy(&cfg);
}
void
init_inputs(void)
{
	int err = init_joysticks(joystick_array);
	if (err == -1)
		exit(EXIT_FAILURE);

}

/*
 * cleanup all create game objects
 */
void
cleanup_game_object(void)
{
	free_game_object(player);

	for (int i = 0; static_obj_array[i] != NULL; i++)
		free_game_object(static_obj_array[i]);
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

	player->func->draw(player->data, renderer);

	for (int i = 0; static_obj_array[i] != NULL; i++)
		static_obj_array[i]->func->draw(static_obj_array[i]->data,
						renderer);

	SDL_RenderPresent(renderer);
}

/*
 *  update all needed stuff
 */
void
update_all(void)
{
	player->func->update(player->data, &player->new_velo);
	player->func->collision_window(player->data, &player->new_velo, &screen);

	for (int i = 0; static_obj_array[i] != NULL; i++)
		player->func->collision_object(player->data,
					static_obj_array[i]->data,
					&player->new_velo);
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
			tip_joystick_axis_move(&e, &player->new_velo, 1);
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
	printf("usage: ./dynamic_conf                                \n");
	printf("       use the joystick to move the astronaut around \n");

	init_game();
	init_inputs();

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
	free_texture_array(texture_array);
	cleanup_main_window(window, renderer);

	exit(EXIT_SUCCESS);
}
