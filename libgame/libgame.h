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

#define eprintf(format, ...) fprintf (stderr, format, ##__VA_ARGS__)

/* frame rate */
#define FPS 60

/* deadzone for the joystick */
#define JOYSTICK_DEADZONE 10000

typedef struct {
	int x;
	int y;
} vector2d_t;

typedef struct {
	int h;
	int w;
} spread_t;

/* this a object within the game */
typedef struct {
	vector2d_t       pos;        /* position                  */
	vector2d_t       velo;       /* velocity                  */
	vector2d_t       accel;      /* acceleration              */
	spread_t         size;       /* height and width          */
	signed char      frame;      /* num frame of spreed sheet */
	SDL_Texture      *texture;
	SDL_RendererFlip flip;
} game_obj_t;

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;   /* alpha */
} color_t;


/*
 * --------------------------- other topics ------------------------------------
 */

/*
 * debug values
 */
void
show_object_kine_vals(game_obj_t *obj);


/*
 * --------------------------- window related ----------------------------------
 */

/*
 * setup main window
 */
SDL_Window *
setup_main_window(char *name, uint32_t size_x, uint32_t size_y, unsigned char f);

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
 * --------------------------- game object related -----------------------------
 */

/*
 * create a game object
 */
game_obj_t *
init_game_object(int x, int y, int w, int h, SDL_Texture *texture);
/* ... from file */
game_obj_t *
init_game_object_from_file(char *filename, int x, int y, int w, int h,
			SDL_Renderer *renderer);
/* ... use texture size */
game_obj_t *
init_game_object_simple(int x, int y, SDL_Texture *texture);

/*
 * free a game object
 */
void
free_game_object(game_obj_t *t);

/*
 * draw a object based on obj content
 */
void
draw_object(game_obj_t *obj, SDL_Renderer *renderer);

/*
 * set game_obj_t.frame with frame
 */
void
set_object_frame(game_obj_t *obj, signed char frame);

/*
 * set accel and calc velo/pos
 */
void
set_object_accel(game_obj_t *obj, vector2d_t *accel);

/*
 * set velo and calc pos
 */
void
set_object_velo(game_obj_t *obj, vector2d_t *velo);

/*
 * get actual value(s) of pos
 */
vector2d_t *
get_object_pos(game_obj_t *obj);
int
get_object_pos_x(game_obj_t *obj);
int
get_object_pos_y(game_obj_t *obj);

/*
 * clear value(s) of pos
 */
void
clear_object_pos_x(game_obj_t *obj);
void
clear_object_pos_y(game_obj_t *obj);
void
clear_object_pos(game_obj_t *obj);

/*
 * get actual value(s) of pos
 */
void
set_object_pos_x(game_obj_t *obj, int x);
void
set_object_pos_y(game_obj_t *obj, int y);
void
set_object_pos_x_y(game_obj_t *obj, int x, int y);
void
set_object_pos(game_obj_t *obj, vector2d_t *pos);
void
set_object_pos_via_mouse(game_obj_t *obj, SDL_Event *e, int frac);

/*
 * get size value(s)
 */
int
get_object_size_w(game_obj_t *obj);
int
get_object_size_h(game_obj_t *obj);
spread_t *
get_object_size(game_obj_t *obj);


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
 * init all alvailabel joysticks
 */
int
init_joysticks(SDL_Joystick *joystick_array[]);

/*
 * close all opened joysticks
 */
void
free_joysticks(SDL_Joystick *joystick_array[]);

/*
 * handle joystick axis movement
 *
 * mov_vec -> accel (or other represantation) vector
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


#endif
