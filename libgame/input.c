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

#include "libgame.h"
#include "libgame_private.h"


LIGGAME_EXPORT void
free_joystick_object_array(game_joystick_t *joystick_array[])
{
	int i = 0;
//	while(joystick_array[i] != NULL) {
//		SDL_JoystickClose(joystick_array);
//
//	}
}

LIGGAME_EXPORT int
init_joysticks(SDL_Joystick *joystick_array[])
{
	if (!SDL_WasInit(SDL_INIT_JOYSTICK))
		SDL_InitSubSystem(SDL_INIT_JOYSTICK);
	else
		printf("SDL_INIT_JOYSTICK == true\n");

	unsigned char n = SDL_NumJoysticks();
	if (!n) {
		printf("no joysticks connected\n");
		return 0;
	} else {
		printf("found %d joysticks\n", n);
	}

	for (int i = 0; i < n; i++) {
		SDL_Joystick *p = SDL_JoystickOpen(i);
		if (p == NULL)
			err_sdl_and_ret("could open joystick", -1);
		else
			joystick_array[0] = p;

		printf("opened joystick %s\n", SDL_JoystickNameForIndex(i));
		printf("axes: %d\nbuttons: %d\nballs: %d\n",
		       SDL_JoystickNumAxes(p), SDL_JoystickNumButtons(p),
		       SDL_JoystickNumBalls(p));
	}

	joystick_array[n] = NULL;

	int err = SDL_JoystickEventState(SDL_ENABLE);
	if (err == -1)
		err_sdl_and_ret("could not enable joystick events", -1);

	return n;
}

LIGGAME_EXPORT void
free_joysticks(SDL_Joystick *joystick_array[])
{
	int i = 0;
	while(joystick_array[i] != NULL)
		SDL_JoystickClose(joystick_array[i++]);
}

LIGGAME_EXPORT void
tip_joystick_axis_move(SDL_Event *e, vector2d_t *mov_vec, unsigned char step)
{
	int value = e->jaxis.value;
	int axis  = e->jaxis.axis;

	if (axis == 0) {
		if (value > JOYSTICK_DEADZONE)
			mov_vec->x = step;           /* right  */
		if (value < -JOYSTICK_DEADZONE)
			mov_vec->x = -step;          /* left   */
	} else {
		if (value > JOYSTICK_DEADZONE)
			mov_vec->y = step;           /* down   */
		if (value < -JOYSTICK_DEADZONE)
			mov_vec->y = -step;          /* up     */
	}
}

LIGGAME_EXPORT void
handle_joystick_axis_move(SDL_Event *e, vector2d_t *mov_vec, unsigned char step)
{
	int value = e->jaxis.value;
	int axis  = e->jaxis.axis;

	if (axis == 0) {
		if (value > JOYSTICK_DEADZONE)
			mov_vec->x = step;           /* right  */
		else if (value < -JOYSTICK_DEADZONE)
			mov_vec->x = -step;          /* left   */
		else
			mov_vec->x = 0;              /* middle */
	} else {
		if (value > JOYSTICK_DEADZONE)
			mov_vec->y = step;           /* down   */
		else if (value < -JOYSTICK_DEADZONE)
			mov_vec->y = -step;          /* up     */
		else
			mov_vec->y = 0;              /* middle */
	}
}

LIGGAME_EXPORT void
tip_keyboard_cursor_move(vector2d_t *mov_vec, unsigned char step)
{
	const uint8_t *k = SDL_GetKeyboardState(NULL);

	if (k[SDL_SCANCODE_RIGHT] || k[SDL_SCANCODE_D])
		mov_vec->x = step;

	if (k[SDL_SCANCODE_LEFT] || k[SDL_SCANCODE_A])
		mov_vec->x = -step;

	if (k[SDL_SCANCODE_UP] || k[SDL_SCANCODE_W])
		mov_vec->y = -step;

	if (k[SDL_SCANCODE_DOWN] || k[SDL_SCANCODE_S])
		mov_vec->y = step;
}

LIGGAME_EXPORT void
handle_keyboard_cursor_move(vector2d_t *mov_vec, unsigned char step)
{
	const uint8_t *k = SDL_GetKeyboardState(NULL);

	if (k[SDL_SCANCODE_RIGHT] || k[SDL_SCANCODE_D]) {
		mov_vec->x = step;
	} else if (k[SDL_SCANCODE_LEFT] || k[SDL_SCANCODE_A]) {
		mov_vec->x = -step;
	} else if (k[SDL_SCANCODE_UP] || k[SDL_SCANCODE_W]) {
		mov_vec->y = -step;
	} else if (k[SDL_SCANCODE_DOWN] || k[SDL_SCANCODE_S]) {
		mov_vec->y = step;
	} else {
		/* clear if other key was pressed */
		mov_vec->y = 0;
		mov_vec->x = 0;
	}
}

LIGGAME_EXPORT void
handle_keyboard_calc_keys(vector2d_t *mov_vec, unsigned char step)
{
	const uint8_t *k = SDL_GetKeyboardState(NULL);

	if (k[SDL_SCANCODE_KP_MINUS] || k[SDL_SCANCODE_MINUS]) {
		printf("MINUS \n");
		scal_sub_vec(mov_vec, step);
	}

	if (k[SDL_SCANCODE_KP_PLUS] || k[SDLK_PLUS]) {
		printf("PLUS \n");
		scal_add_vec(mov_vec, step);
	}
}

LIGGAME_EXPORT void
get_mouse_position(SDL_Event *e, vector2d_t *pos)
{
	pos->x = e->motion.x;
	pos->y = e->motion.y;

	printf("mouse click @x: %d and @y: %d\n", pos->x, pos->y);
}

LIGGAME_EXPORT void
get_mouse_position_x(SDL_Event *e, int *pos_x)
{
	*pos_x = e->motion.x;

	printf("mouse click @x: %d\n", *pos_x);
}

LIGGAME_EXPORT void
get_mouse_position_y(SDL_Event *e, int *pos_y)
{
	*pos_y = e->motion.y;

	printf("mouse click @y: %d\n", *pos_y);
}

LIGGAME_EXPORT void
get_mouse_position_x_y(SDL_Event *e, int *pos_x, int *pos_y)
{
	*pos_x = e->motion.x;
	*pos_y = e->motion.y;

	printf("mouse click @x: %d and @y: %d\n", *pos_x, *pos_y);
}
