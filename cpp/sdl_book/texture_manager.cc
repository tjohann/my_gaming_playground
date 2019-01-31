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

Texture_manager* Texture_manager::instance_ = 0;

bool Texture_manager::load(std::string filename,
			   std::string id,
			   SDL_Renderer *renderer)
{
	SDL_Surface *tmp_surface = IMG_Load(filename.c_str());
	if (tmp_surface == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,
			     "could not load image: %s",
			     SDL_GetError());
		return false;
	}

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer,
							    tmp_surface);
        if (texture == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,
			     "could not create texture from surface: %s",
			     SDL_GetError());
		return false;
	} else {
		texture_map[id] = texture;
	}

	SDL_FreeSurface(tmp_surface);
        SDL_Log("texture %s loaded", filename.c_str());

	return true;
}

void Texture_manager::draw(std::string id,
			   int x, int y, int w, int h,
			   SDL_Renderer *renderer,
			   SDL_RendererFlip flip)
{

	SDL_Rect src_rect;
	SDL_Rect dest_rect;

	src_rect.x = 0;
	src_rect.y = 0;
	src_rect.w = dest_rect.w = w;
	src_rect.h = dest_rect.h = h;

	dest_rect.x = x;
	dest_rect.y = y;

	SDL_Texture *texture = texture_map[id];
	if (texture == NULL) {
		std::cout << "no valid texture for " << id
			  << " available" << std::endl;
	} else {
		int ret = SDL_RenderCopyEx(renderer, texture,
					   &src_rect, &dest_rect, 0, 0, flip);
		if (ret != 0)
			SDL_LogError(SDL_LOG_CATEGORY_ERROR,
				     "unable to draw image %s",
				     SDL_GetError());
	}
}

void Texture_manager::draw_frame(std::string id,
				 int x, int y, int w, int h,
				 int curr_row, int curr_frame,
				 SDL_Renderer *renderer,
				 SDL_RendererFlip flip)
{
	SDL_Rect src_rect;
	SDL_Rect dest_rect;

	src_rect.x = w * curr_frame;
	src_rect.y = h * (curr_row -1);
	src_rect.w = dest_rect.w = w;
	src_rect.h = dest_rect.h = h;

	dest_rect.x = x;
	dest_rect.y = y;

	SDL_Texture *texture = texture_map[id];
	if (texture == NULL) {
		std::cout << "no valid texture for "
			  << id << " available" << std::endl;
	} else {
		int ret = SDL_RenderCopyEx(renderer, texture,
					   &src_rect, &dest_rect, 0, 0, flip);
		if (ret != 0)
			SDL_LogError(SDL_LOG_CATEGORY_ERROR,
				     "unable to draw image %s",
				     SDL_GetError());
	}
}
