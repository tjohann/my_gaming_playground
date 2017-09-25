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

#define PLAYER_PIC "astronaut_side.png"
#define STATIC_OBJ_PIC "astronaut.png"

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
game_obj_t *player;

/* the fixed objects */
game_obj_t **static_obj_array;

/* the texture array */
game_texture_t **texture_array;

/* all joysticks */
#define MAX_NUM_JOYSTICKS 1
SDL_Joystick *joystick_array[MAX_NUM_JOYSTICKS + 1];

/* size of window */
char *progname = "dynamic_config";
char *config_file = "dynamic_config.conf";

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;


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
	window = setup_main_window_via_config(&cfg, 0);
	if (window == NULL)
		exit(EXIT_FAILURE);

	renderer = setup_renderer_via_config(&cfg, window);
	if (renderer == NULL)
		exit(EXIT_FAILURE);

	/* load all textures */
	err = load_texture_via_config(&cfg, texture_array, renderer);
	if (err == -1)
		exit(EXIT_FAILURE);

	config_destroy(&cfg);
}
void
init_game_objects(void)
{
	SDL_Texture *texture = load_texture(PLAYER_PIC, renderer);
	if (texture == NULL)
		exit(EXIT_FAILURE);

	/* player object */
	game_obj_t *t = alloc_game_object_simple("player", 0, SCREEN_HEIGHT/2,
						texture);
	if (t == NULL)
		exit(EXIT_FAILURE);
	else
		player = t;

	/* the static objects */

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

	player->func->draw(player->data, renderer);  /* use the member func */

//	for (int i = 0; static_obj_array[i] != NULL; i++)
//		static_obj_array[i]->func->draw(static_obj_array[i]->data,
//						renderer);

	SDL_RenderPresent(renderer);
}

/*
 *  update all needed stuff
 */
void
update_all(void)
{
	player->func->update(player->data, &player->new_velo);
	player->func->collision_window(player->data, &player->new_velo,
				SCREEN_WIDTH, SCREEN_HEIGHT);

//	for (int i = 0; static_obj_array[i] != NULL; i++)
//		player->func->collision_object(player->data,
//					static_obj_array[i]->data, &player->new_velo);
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
	printf("usage: ./collision                                   \n");
	printf("       use the joystick to move the astronaut around \n");

	init_game();
	init_game_objects();

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
	cleanup_main_window(window, renderer);

	exit(EXIT_SUCCESS);
}
