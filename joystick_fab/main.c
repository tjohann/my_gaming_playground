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

char *progname = "joystick_fab";
char *config_file = "joystick_fab.conf";

SDL_Window   *window;
SDL_Renderer *renderer;

int screen_width;
int screen_height;

game_texture_t *texture_array;

game_obj_t **players;               /* the player parts          */
game_obj_t **static_objs;           /* the fixed objects         */
game_obj_t **enemies;               /* the enemies flying around */

/* all joysticks */
#define MAX_NUM_JOYSTICKS 2
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
	window = setup_main_window_via_config(&cfg, 0,
					&screen_width, &screen_height);
	if (window == NULL)
		exit(EXIT_FAILURE);

	renderer = setup_renderer_via_config(&cfg, window);
	if (renderer == NULL)
		exit(EXIT_FAILURE);

	/* load all textures */
	texture_array = alloc_textures_via_config(&cfg, renderer);
	if (texture_array == NULL)
		exit(EXIT_FAILURE);

	/* alloc all player objects */
	players = alloc_player_objects_via_config(&cfg, texture_array);
	if (players == NULL)
		exit(EXIT_FAILURE);

	for (int i = 0; players[i] != NULL; i++)
		printf("name %s\n", players[i]->name);

	/* alloc all static objects */
	static_objs = alloc_static_objects_via_config(&cfg, texture_array);
	if (static_objs == NULL)
		exit(EXIT_FAILURE);

	for (int i = 0; static_objs[i] != NULL; i++)
		printf("name %s\n", static_objs[i]->name);

	enemies = alloc_enemie_objects_via_config(&cfg, texture_array);
	if (enemies == NULL)
		exit(EXIT_FAILURE);

	for (int i = 0; enemies[i] != NULL; i++)
		printf("name %s\n", enemies[i]->name);

	config_destroy(&cfg);
}

/*void
init_inputs(void)
{
	int err = init_joysticks(joystick_array);
	if (err == -1)
		exit(EXIT_FAILURE);

		}*/

/*
 * cleanup all create game objects
 */
void
cleanup_game_object(void)
{
	for (int i = 0; players[i] != NULL; i++)
		free_game_object(players[i]);

	for (int i = 0; static_objs[i] != NULL; i++)
		free_game_object(static_objs[i]);

	for (int i = 0; enemies[i] != NULL; i++)
		free_game_object(enemies[i]);
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

	for (int i = 0; static_objs[i] != NULL; i++)
		static_objs[i]->func->draw(static_objs[i]->data, renderer);

	for (int i = 0; enemies[i] != NULL; i++)
		enemies[i]->func->draw(enemies[i]->data, renderer);

	SDL_RenderPresent(renderer);
}

/*
 *  update all needed stuff
 */
void
update_all(void)
{
	for (int i = 0; players[i] != NULL; i++) {
		players[i]->func->update(players[i]->data, &
					players[i]->new_velo);
		players[i]->func->collision_window(players[i]->data,
						&players[i]->new_velo,
						screen_width,
						screen_height);

		for (int j = 0; static_objs[j] != NULL; j++)
			players[i]->func->collision_object(players[i]->data,
							static_objs[j]->data,
							&players[i]->new_velo);

		for (int j = 0; enemies[j] != NULL; j++)
			if (players[i]->func->detect_collision_object(
					players[i]->data,
					enemies[j]->data))
				printf("COOOOOLISSION with %s\n", enemies[j]->name);
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
			printf("quit the example -> bye %s\n", getenv("USER"));
			running = false;
			break;
		case SDL_JOYAXISMOTION:
			printf("SDL_JOYAXISMOTION\n");
			unsigned char which = e.jaxis.which;
			if (which == 0 && players[0] != NULL)
				tip_joystick_axis_move(&e, &players[0]->new_velo, 1);
			else if (which == 1 && players[1] != NULL)
				tip_joystick_axis_move(&e, &players[1]->new_velo, 1);
			else
				printf("no valid joystick handling\n");
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
	printf("usage: ./joystick_fab                                \n");
	printf("       use the joystick to move the astronaut around \n");
	printf("       the static objects redirect your movement     \n");
	printf("       a collision with the enemies will be shown    \n");

	init_game();
//	init_inputs();

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
//	free_joysticks(joystick_array);
	cleanup_main_window(window, renderer);

	exit(EXIT_SUCCESS);
}
