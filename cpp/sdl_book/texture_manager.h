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

#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H

#include <iostream>
#include <string>
#include <map>

#include <SDL.h>


class Texture_manager
{
public:
	Texture_manager();
	~Texture_manager();

	bool load(std::string filename,
		  std::string id,
		  SDL_Renderer *renderer);

	void draw(std::string id,
		  int x, int y, int w, int h,
		  SDL_Renderer *renderer,
		  SDL_RendererFlip flip = SDL_FLIP_NONE);

	void draw_frame(std::string id,
			int x, int y, int w, int h,
			int curr_row, int curr_frame,
			SDL_Renderer *renderer,
			SDL_RendererFlip flip = SDL_FLIP_NONE);

private:
	std::map<std::string, SDL_Texture*> texture_map;
};


#endif
