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

typedef struct spread_t {
	int h;
	int w;
} spread_t;

/* hold all textures */
typedef struct {
	char          *name;
	SDL_Texture   *texture;

	/* TODO: add handling of it */
	union {
		struct spread_t;
		spread_t size;
	};

	unsigned char max_frames_x;          /* num columns                   */
	unsigned char max_frames_y;          /* num rows                      */
} game_texture_t;

/* simple game struct */
typedef struct {
	char *name;
	char *config;

	SDL_Window   *window;
	SDL_Renderer *renderer;

	union {
		struct spread_t;            /* screen size                    */
		spread_t screen;
	};

	game_texture_t  *texture_array;     /* all used textures              */
	size_t size_texture_array;
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
