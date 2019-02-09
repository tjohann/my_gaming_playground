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


/*
 * --------------------------- texture related ---------------------------------
 */


/*
 * --------------------------- window related ----------------------------------
 */


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
