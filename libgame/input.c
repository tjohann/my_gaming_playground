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
		SDL_Joystick *p = SDL_JoystickOpen(0);
		if (p == NULL)
			err_sdl_and_ret("could open joystick", -1);
		else
			joystick_array[0] = p;

		printf("opened joystick %s\n", SDL_JoystickNameForIndex(i));
		printf("axes: %d\tbuttons: %d\tballs: %d\n",
			SDL_JoystickNumAxes(p), SDL_JoystickNumButtons(p), SDL_JoystickNumBalls(p));
	}

	int err = SDL_JoystickEventState(SDL_ENABLE);
	if (err == -1)
		err_sdl_and_ret("could not enable joystick events", -1);

	return n;
}
