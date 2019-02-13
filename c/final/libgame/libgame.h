/*
  GPL
  (c) 2017-2019, thorsten.johannvorderbrueggen@t-online.de

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

#ifndef __USE_GNU
#define __USE_GNU
#endif
#define _GNU_SOURCE

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

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

typedef struct pos2d_t{
	int x;
	int y;
} pos2d_t;

typedef struct spread_t {
	int h;
	int w;
} spread_t;

/* hold all infos around the textures */
typedef struct {
	char          *name;
	SDL_Texture   *texture;

	/* TODO: add handling of it */
	union {
		struct spread_t;
		spread_t size;               /* size of ONE sprite             */
	};

	unsigned char max_frames_x;          /* num columns                    */
	unsigned char max_frames_y;          /* num rows                       */
} game_texture_t;

/* the game object */
typedef struct {
	char             *name;
	game_texture_t   *texture_obj;

	union {
		struct pos2d_t;
		pos2d_t pos;
	};

	vector2d_t       velo;
	vector2d_t       accel;

	unsigned char    column;     /* which sprite for the animation         */
	unsigned char    row;        /* row represents front/back/left/right   */

	SDL_RendererFlip flip;
} game_obj_t;

/* the game struct -> holds all infos needed for the game */
typedef struct {
	char *name;
	char *config;

	SDL_Window   *window;
	SDL_Renderer *renderer;

	union {
		struct spread_t;            /* screen size                     */
		spread_t screen;
	};

	game_texture_t  *textures_array;    /* NULL is vaild, if the textures  */
	size_t size_textures_array;         /* are included via config.tilesets*/

	game_obj_t *players;                /* the player objects              */
	size_t size_players_array;

	game_obj_t *enemies;                /* the enemies objects             */
	size_t size_enemies_array;

	game_obj_t *objects;                /* means WITH movement             */
	size_t size_objects_array;

	game_obj_t *static_objects;         /* means NO movement, but          */
	size_t size_static_objects_array;   /* animation is possible           */

} game_t;

/*
 * --------------------------- other topics -------------------------------------
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
 * --------------------------- game related -------------------------------------
 */

int
init_game(game_t *game);

void
cleanup_game(game_t *game);


/*
 * --------------------------- texture related ----------------------------------
 */

SDL_Texture *
load_texture(char *file, SDL_Renderer *r);

void
destroy_texture(SDL_Texture *t);


/*
 * --------------------------- window related -----------------------------------
 */

SDL_Window *
setup_window(const char *name, spread_t *screen, unsigned int flags);

SDL_Renderer *
setup_renderer(SDL_Window *w, color_t *b, unsigned int flags);

void
cleanup_window(SDL_Window *w, SDL_Renderer *r);



/*
 * --------------------------- game object related ------------------------------
 */

game_obj_t *
alloc_game_object(char *name, int x, int y, game_texture_t *t);

void
free_game_object(game_obj_t *t);



/*
 * --------------------------- collision related --------------------------------
 */


/*
 * --------------------------- 2d vector related --------------------------------
 */


/*
 * --------------------------- input related ------------------------------------
 */


#endif
