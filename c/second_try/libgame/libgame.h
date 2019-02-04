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

#ifndef _LIBGAME_H_
#define _LIBGAME_H_

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

/* for libconfig */
#include <libconfig.h>

#define eprintf(format, ...) fprintf (stderr, format, ##__VA_ARGS__)

/* base frame rate */
#define FPS 60

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;   /* alpha */
} color_t;

typedef struct {
	float x;
	float y;
} vector2d_t;

typedef struct {
	int x;
	int y;
} pos2d_t;

typedef struct {
	int h;
	int w;
} spread_t;


/* object specific data */
typedef struct {
	pos2d_t          pos;
	spread_t         size;

	vector2d_t       velo;

	unsigned char    frame_x;
	unsigned char    max_frames_x;

	unsigned char    frame_y;
	unsigned char    max_frames_y;

	unsigned char    orientation;
	SDL_RendererFlip flip;
} game_obj_data_t;


/* object specific update struct */
typedef struct {
	vector2d_t    velo;

	unsigned char frame_x;
	unsigned char frame_y;

	unsigned char orientation;
} game_obj_event_data_t;

/* the member functions */
typedef void (*draw_func) (game_obj_data_t *t, SDL_Texture *texture,
			   SDL_Renderer *renderer);
typedef void (*update_func) (game_obj_data_t *t, vector2d_t *velo);
typedef void (*collision_window_func) (game_obj_data_t *t, vector2d_t *velo,
				       spread_t *size);
typedef void (*collision_object_func) (game_obj_data_t *a, game_obj_data_t *b,
				       vector2d_t *velo);
typedef bool (*detect_collision_object_func) (game_obj_data_t *a,
					      game_obj_data_t *b);
typedef void (*joystick_axis_func) (SDL_Event *e, vector2d_t *mov_vec,
				    unsigned char step);

/* object type specific member functions */
typedef struct {
	joystick_axis_func           handle_joystick_axis;
	/*
        joystick_button_func         handle_joystick_button;
	keyboard_axis_func           handle_keyboard_axis;
	keyboard_botton_func         handle_keybord_button;
	*/

	update_func                  update;
	/* TODO:
        ai_func                      ai;
	animate_func                 animate */

	detect_collision_object_func detect_collision_object;
	/* TODO:
	detect_collision_window_func detect_collision_window; */

	collision_object_func        collision_object;
	collision_window_func        collision_window;
} game_obj_func_t;

/* the game object */
typedef struct {
	char                         *name;
	SDL_Texture                  *texture;

	game_obj_data_t              *data;            /* filled in update_all */
	game_obj_event_data_t        *event_data;   /* filled in handle_events */

	game_obj_func_t              *func;  /* only needed for mobile objects */
	draw_func                    draw;             /* use act_data to draw */
} game_obj_t;


/* hold all textures */
typedef struct {
	char          *name;
	SDL_Texture   *texture;

	/* TODO: add handling of it */
	spread_t      size;
	unsigned char max_frames_x;
	unsigned char max_frames_y;
} game_texture_t;

/* hold all joysticks and the config */
typedef struct {
	char               *name;
	char               *player;
	unsigned char      step;
	SDL_Joystick       *joystick;
	vector2d_t         *to_change;          /* pointer to value to change */
	joystick_axis_func handle_axis_joystick;          /* handle axis move */
} game_joystick_t;


/* simple game struct */
typedef struct {
	char *name;
	char *config;

	SDL_Window   *window;
	SDL_Renderer *renderer;

	spread_t screen;                    /* screen size                    */

	game_texture_t  *texture_array;     /* all used textures              */
	size_t size_texture_array;

	game_joystick_t *joystick_array;    /* all used joysticks             */
	size_t size_joystick_array;

	game_obj_t *players;                /* the player objects             */
	size_t size_players_array;

	game_obj_t *enemies;                /* the enemies objects            */
	size_t size_enemies_array;

	game_obj_t *objects;                /* the objects with actions       */
	size_t size_objects_array;

	game_obj_t *static_objects;         /* the static objects             */
	size_t size_static_objects_array;
} game_t;


/*
 * --------------------------- other topics ------------------------------------
 */

/*
 * string topics
 */
char *
alloc_string(const char *s);

/*
 * other topics
 */
int
get_random_value(void);


/*
 * --------------------------- game related ------------------------------------
 */

int
init_game(game_t *game);

void
cleanup_game(game_t *game);


/*
 * --------------------------- texture related ---------------------------------
 */

SDL_Texture *
load_texture(char *file, SDL_Renderer *r);

void
destroy_texture(SDL_Texture *t);


/*
 * --------------------------- window related ----------------------------------
 */

SDL_Window *
setup_window(const char *name, spread_t *screen, unsigned int flags);

SDL_Renderer *
setup_renderer(SDL_Window *w, color_t *b, unsigned int flags);

void
cleanup_window(SDL_Window *w, SDL_Renderer *r);



/*
 * --------------------------- object fab related ------------------------------
 */


/*
 * --------------------------- game object related -----------------------------
 */

game_obj_t *
alloc_game_object(char *name, int x, int y, game_texture_t *t);

void
free_game_object(game_obj_t *t);

void
draw_game_object(game_obj_t *t, SDL_Renderer *renderer);


/*
 * --------------------------- collision related -------------------------------
 */


/*
 * --------------------------- 2d vector related -------------------------------
 */


/*
 * --------------------------- input related -----------------------------------
 */


#endif
