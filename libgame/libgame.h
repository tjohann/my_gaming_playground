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


typedef struct {
	int x;
	int y;
} pos_t;

typedef struct {
	int h;
	int w;
} spread_t;

/* this a object within the game */
typedef struct {
	pos_t            pos;
	spread_t         size;
	signed char      frame;
	SDL_Texture      *texture;
	SDL_RendererFlip flip;
} game_obj_t;

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
} color_t;


/*
 * --------------------------- other topics ------------------------------------
 */

/*
 * setup main window
 */
SDL_Window *
setup_main_window(char *name, int size_x, int size_y, unsigned char f);

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
 * --------------------------- 2d vector related -------------------------------
 */



#endif
