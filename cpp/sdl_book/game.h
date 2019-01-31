/*
  GPL
  (c) 2019, thorsten.johannvorderbrueggen@t-online.de

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

#ifndef _GAME_H_
#define _GAME_H

#include <SDL.h>
#include <SDL_log.h>

#include "texture_manager.h"

class Game
{
public:
	Game();
	~Game();

	void init(const char* title, int x, int y, int w, int h,
		bool fullscreen);
	void cleanup_all();

	void render_all();
	void update_all();
	void handle_events();

	bool is_running() { return running; }

private:
	SDL_Window *window;
	SDL_Renderer *renderer;

        int current_frame;
	Texture_manager texture_manager;

	bool running;
};


#endif
