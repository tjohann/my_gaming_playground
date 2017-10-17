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

/* for libconfig */
#include <libconfig.h>

/* for libxml2 */
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#define eprintf(format, ...) fprintf (stderr, format, ##__VA_ARGS__)

/* base frame rate */
#define FPS 60

/* deadzone for the joystick */
#define JOYSTICK_DEADZONE 10000


typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;   /* alpha */
} color_t;

typedef struct {
	int x;
	int y;
} vector2d_t;

typedef struct {
	int h;
	int w;
} spread_t;

/* a object within the game ... or only the data */
typedef struct {
	vector2d_t       pos;        /* position                  */
	vector2d_t       velo;       /* velocity                  */
	spread_t         size;       /* height and width          */
	signed char      frame;      /* num frame of sprite sheet */
	SDL_Texture      *texture;
	SDL_RendererFlip flip;
} game_obj_data_t;

/* the member functions */
typedef void (*draw_func) (game_obj_data_t *t, SDL_Renderer *renderer);
typedef void (*update_func) (game_obj_data_t *t, vector2d_t *velo);
typedef void (*collision_window_func) (game_obj_data_t *t, vector2d_t *velo,
				const int w, const int h);
typedef void (*collision_object_func) (game_obj_data_t *a, game_obj_data_t *b,
				vector2d_t *velo);
typedef bool (*detect_collision_object_func) (game_obj_data_t *a,
					game_obj_data_t *b);
typedef void (*joystick_axis_func) (SDL_Event *e, vector2d_t *mov_vec,
				unsigned char step);

typedef struct {
	draw_func                    draw;
	update_func                  update;
	collision_window_func        collision_window;
	collision_object_func        collision_object;
	detect_collision_object_func detect_collision_object;
	joystick_axis_func           handle_axis_joystick;
} game_obj_func_t;

/* a object within the game ... as a container for data and func */
typedef struct {
	game_obj_data_t *data;
	game_obj_func_t *func;
	char            *name;
	vector2d_t      new_velo;   /* velo to set in next update */
} game_obj_t;


/* hold all textures */
typedef struct {
	char        *name;
	SDL_Texture *texture;
} game_texture_t;

/* hold all joysticks and the config */
typedef struct {
	char               *name;
	char               *player;
	unsigned char      step;
	SDL_Joystick       *joystick;
	vector2d_t         *to_change;          /* pointer to value to change */
	joystick_axis_func handle_axis_joystick;/* handle axis move           */
} game_joystick_t;


/* the flags for init_game_via_config */
#define INIT_PLAYERS 0x01
#define INIT_OBJECTS 0x02
#define INIT_ENIMIES 0x04
/* simple game struct */
typedef struct {
	char *progname;
	char *config_file;

	SDL_Window   *window;
	SDL_Renderer *renderer;

	int screen_width;
	int screen_height;

	game_texture_t  *texture_array;     /* all used textures         */
	game_joystick_t *joystick_array;    /* all used joysticks        */

	game_obj_t **players;               /* the player parts          */
	game_obj_t **static_objs;           /* the fixed objects         */
	game_obj_t **enemies;               /* the enemies flying around */
} game_t;


/*
 * --------------------------- object fab related ------------------------------
 *
 * use libconfig to generate all relevant objects from a config file
 */

/*
 * init all parts of the game
 */
int
init_game_via_config(game_t *game, unsigned char flags);

/*
 * alloc an array of all player objects
 */
game_obj_t **
alloc_objects_via_config(config_t *cfg, char *section,
			game_texture_t textures[]);

game_obj_t **
alloc_player_objects_via_config(config_t *cfg, game_texture_t textures[]);

game_obj_t **
alloc_static_objects_via_config(config_t *cfg, game_texture_t textures[]);

game_obj_t **
alloc_enemie_objects_via_config(config_t *cfg, game_texture_t textures[]);


/* read config and check if its for this game */
int
open_config(char *file, char *name, config_t *cfg);

/* setup main window ... via configuration file */
SDL_Window *
setup_main_window_via_config(config_t *cfg, unsigned char flags,
			int *screen_width, int *screen_height);

/* setup renderer via configuration file */
SDL_Renderer *
setup_renderer_via_config(config_t *cfg, SDL_Window *window);

/* create array game_texture_t array based on cofiguration file */
game_texture_t *
alloc_textures_via_config(config_t *cfg, SDL_Renderer *renderer);

/* alloc an array with all joysticks */
game_joystick_t *
alloc_joystick_objects_via_config(config_t *cfg);


/*
 * --------------------------- object fab related ------------------------------
 *
 * use libxml to generate all relevant objects from a xml config file
 */

/*
 * TODO: -> do same like object_fab.c with object_fab_xml.c
 */


/*
 * --------------------------- other topics ------------------------------------
 */

/*
 * debug values
 */

/*
 * calls the game_obj_data_t based functions
 */
void
show_object_vals(game_obj_t *obj);
void
show_object_kine_vals(game_obj_data_t *obj);
void
show_object_size_vals(game_obj_data_t *obj);

/*
 * string stuff
 */

char *
alloc_string(const char *s);


/*
 * --------------------------- window related ----------------------------------
 */

/*
 * setup main window
 */
SDL_Window *
setup_main_window(const char *name, uint32_t size_x, uint32_t size_y,
		unsigned char flags);

/*
 * setup renderer
 */
SDL_Renderer *
setup_renderer(SDL_Window *window, color_t *b);

/*
 * cleanup window, renderer and SDL_Quit
 */
void
cleanup_main_window(SDL_Window *window, SDL_Renderer *renderer);


/*
 * --------------------------- texture related ---------------------------------
 */

/*
 * create a texture from file
 */
SDL_Texture *
load_texture(char *file_name, SDL_Renderer *renderer);

/*
 * free/clear all allocated mem of texture array
 */
void
free_texture_array(game_texture_t array[]);
void
clear_texture_array(game_texture_t array[]);


/*
 * --------------------------- game object related -----------------------------
 */

/*
 * create a game object (the complex game object with function pointers)
 */

/* ... use texture size */
game_obj_t *
alloc_game_object_simple(char *name, int x, int y, SDL_Texture *texture);
/* ... from texture array */
game_obj_t *
alloc_game_object_from_array(char *name, char *texture_name, int x, int y,
			game_texture_t a[]);

/*
 * free a game data object
 */
void
free_game_object(game_obj_t *obj);

/*
 * create a games data object (the simple game object without function pointers)
 */
game_obj_data_t *
alloc_game_data_object(int x, int y, int w, int h, SDL_Texture *texture);
/* ... use texture size */
game_obj_data_t *
alloc_game_data_object_simple(int x, int y, SDL_Texture *texture);
/* ... from file */
game_obj_data_t *
alloc_game_data_object_from_file(char *filename, int x, int y, int w, int h,
				SDL_Renderer *renderer);
/* ... from file ... use texture size */
game_obj_data_t *
alloc_game_data_object_from_file_simple(char *filename, int x, int y,
					SDL_Renderer *renderer);

/*
 * free a games data object
 */
void
free_game_data_object(game_obj_data_t *obj);

/*
 *  a games func object
 */
game_obj_func_t *
alloc_game_func_object_simple(void);

/*
 * free a games func object
 */
void
free_game_func_object(game_obj_func_t *obj);

/*
 * draw a object based on obj content
 */
void
draw_object(game_obj_data_t *obj, SDL_Renderer *renderer);

/*
 * set game_obj_data_t.frame with frame
 */
void
set_object_frame(game_obj_data_t *obj, signed char frame);

/*
 * set velo and calc pos
 */
void
set_object_velo(game_obj_data_t *obj, vector2d_t *velo);

/*
 * get actual velo values
 */
vector2d_t *
get_object_velo(game_obj_data_t *obj);
int
get_object_velo_x(game_obj_data_t *obj);
int
get_object_velo_y(game_obj_data_t *obj);

/*
 * get actual value(s) of pos
 */
vector2d_t *
get_object_pos(game_obj_data_t *obj);
int
get_object_pos_x(game_obj_data_t *obj);
int
get_object_pos_y(game_obj_data_t *obj);

/*
 * clear value(s) of pos
 */
void
clear_object_pos_x(game_obj_data_t *obj);
void
clear_object_pos_y(game_obj_data_t *obj);
void
clear_object_pos(game_obj_data_t *obj);

/*
 * get actual value(s) of pos
 */
void
set_object_pos_x(game_obj_data_t *obj, int x);
void
set_object_pos_y(game_obj_data_t *obj, int y);
void
set_object_pos_x_y(game_obj_data_t *obj, int x, int y);
void
set_object_pos(game_obj_data_t *obj, vector2d_t *pos);
void
set_object_pos_via_mouse(game_obj_data_t *obj, SDL_Event *e, int frac);

/*
 * get size value(s)
 */
int
get_object_size_w(game_obj_data_t *obj);
int
get_object_size_h(game_obj_data_t *obj);
spread_t *
get_object_size(game_obj_data_t *obj);

/*
 * get the actual positions of the object surface
 */
void
calc_object_surface_pos(game_obj_data_t *obj, int *l, int *r, int *t, int *b);


/*
 * --------------------------- collision related -------------------------------
 */

/*
 * check if pos is out of w/h and inv velo values
 */
void
collision_window(game_obj_data_t *obj, vector2d_t *velo,
		const int w, const int h);

/*
 * check if object a and b collide inv velo values
 */
void
collision_object(game_obj_data_t *a, game_obj_data_t *b, vector2d_t *velo);

/*
 * check if object a and b collide
 */
bool
detect_collision_object(game_obj_data_t *a, game_obj_data_t *b);


/*
 * --------------------------- 2d vector related -------------------------------
 */

/*
 * base operations
 */
void
add_vec(vector2d_t *a, vector2d_t *b);     /* a += b*/
void
sub_vec(vector2d_t *a, vector2d_t *b);     /* a -= b*/

/*
 * scalar operations
 */
void
scal_mul_vec(vector2d_t *a, int b);
void
scal_div_vec(vector2d_t *a, int b);
void
scal_sub_vec(vector2d_t *a, int b);
void
scal_add_vec(vector2d_t *a, int b);

/*
 * invert x/y
 */
void
inv_vec(vector2d_t *a);
void
inv_vec_x(vector2d_t *a);
void
inv_vec_y(vector2d_t *a);

/*
 * others
 */
int
lenght_vec(vector2d_t *a);
void
norm_vec(vector2d_t *a);
void
clear_vec(vector2d_t *a);


/*
 * --------------------------- input related -----------------------------------
 */

/*
 * get number of connected joysticks
 */
int
get_num_joysticks(void);

/*
 * free/clear all game_joystick_t array and close all joysticks
 */
void
free_joystick_array(game_joystick_t array[]);
void
free_joystick_array(game_joystick_t array[]);

/*
 * connect player object with joystick object
 */
void
connect_player_objects(game_joystick_t array[], game_obj_t *players[]);

/*
 * set a different joystick handler function
 */
void
set_joystick_handler(char *player, game_joystick_t array[],
		game_obj_t *players[], joystick_axis_func handle_axis_joystick);

/*
 * init/close all availabel joysticks
 */
int
init_joysticks(SDL_Joystick *joystick_array[]);
void
free_joysticks(SDL_Joystick *joystick_array[]);

/*
 * handle joystick axis movement
 *
 * mov_vec -> velo (or other represantation) vector
 * step    -> set mov_vec.x/y to step
 */
void
handle_joystick_axis_move(SDL_Event *e, vector2d_t *mov_vec,
			unsigned char step);

/*
 * handle joystick axis movement
 *
 * same like handle_joystick_axis_move, but wont clear mov_vec with
 * middle pos!
 */
void
tip_joystick_axis_move(SDL_Event *e, vector2d_t *mov_vec,
		unsigned char step);


/*
 * handle cursor keys for movement
 */
void
handle_keyboard_cursor_move(vector2d_t *mov_vec, unsigned char step);

/*
 * handle cursor keys for movement
 *
 * same like handle_keyboard_cursor_move, but wont clear mov_vec with
 * unknown keys!
 */
void
tip_keyboard_cursor_move(vector2d_t *mov_vec, unsigned char step);

/*
 * handle cursor keys for movement
 */
void
handle_keyboard_calc_keys(vector2d_t *mov_vec, unsigned char step);


/*
 * get the actual position of the mouse cursor
 */
void
get_mouse_position(SDL_Event *e, vector2d_t *pos);
void
get_mouse_position_x(SDL_Event *e, int *pos_x);
void
get_mouse_position_y(SDL_Event *e, int *pos_y);
void
get_mouse_position_x_y(SDL_Event *e, int *pos_x, int *pos_y);


/*
 * --------------------------- other topics ------------------------------------
 */


#endif
