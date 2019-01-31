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

#include "texture_manager.h"


bool load(std::string filename,
	  std::string id,
	  SDL_Renderer *renderer)
{
	SDL_Surface *tmp_surface = IMG_Load("animate-alpha.png");
	if (tmp_surface == NULL)
	  return false;

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, tmp_surface);
	if (texture != NULL)
	  texture_map[id] = texture;
	else
	  goto error;
	  
	SDL_FreeSurface(tmp_surface);

	
 error:
	SDL_LogError(SDL_LOG_CATEGORY_ERROR,
		     "an error occured: %s",
		     SDL_GetError());
	
	return false;
}

void draw(std::string id,
		  int x, int y, int w, int h,
		  SDL_Renderer *renderer,
		  SDL_RendererFlip flip = SDL_FLIP_NONE)
{

  SDL_Rect src_rect;
  SDL_Rect dest_rect;
  
}

void draw_frame(std::string id,
		int x, int y, int w, int h,
		int curr_row, int curr_frame,
		SDL_Renderer *renderer,
		SDL_RendererFlip flip = SDL_FLIP_NONE)
{

}
