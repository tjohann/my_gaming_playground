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

#include "game.h"


Game::Game()
{
	/* do something */
}


Game::~Game()
{
	/* do something */
}


void Game::init(const char* title,
		int x, int y, int w, int h, bool fullscreen)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,
			     "unable to initialize SDL: %s",
			     SDL_GetError());
		exit(EXIT_FAILURE);
	} else {
		SDL_Log("SDL initialised");
	}

	int flags = 0;
	if (fullscreen)
		flags = SDL_WINDOW_FULLSCREEN;

	int xpos = 0;
	if (x == 0)
		xpos = SDL_WINDOWPOS_UNDEFINED;
	else
		xpos = x;

	int ypos = 0;
	if (y == 0)
		ypos = SDL_WINDOWPOS_UNDEFINED;
	else
		ypos = y;

	window = SDL_CreateWindow(title,
					xpos, ypos,
					w, h,
					flags);
	if (window == 0) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,
			     "unable to initialize window: %s",
			     SDL_GetError());
		exit(EXIT_FAILURE);
	}

        /*
         * -1 init the first on supporting
         * 0 (try first SDL_RENDERER_ACCELERATED)
         */
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == 0) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,
			     "unable to initialize renderer: %s",
			     SDL_GetError());
		exit(EXIT_FAILURE);
	}

        // RGB and alpha -> set to black
        int ret = SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        if (ret != 0) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,
			     "unable to set background color: %s",
			     SDL_GetError());
		exit(EXIT_FAILURE);
	}

	/*
	 * will be move to texture manager
	 */
	SDL_Surface *tmp_surface = IMG_Load("animate-alpha.png");
	if (tmp_surface == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,
			     "unable to load bitmap animate.bmp: %s",
			     SDL_GetError());
		exit(EXIT_FAILURE);
	}

	texture = SDL_CreateTextureFromSurface(renderer, tmp_surface);
	if (texture == NULL){
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,
			     "unable to create texture of bitmap rider.bmp: %s",
			     SDL_GetError());
		exit(EXIT_FAILURE);
	}

	SDL_FreeSurface(tmp_surface);

	// get the size of the complete sprite
	int tmp_w = 0, tmp_h = 0;
	ret = SDL_QueryTexture(texture, NULL, NULL, &tmp_w, &tmp_h);
	if (ret != 0) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,
			     "unable to query texture: %s",
			     SDL_GetError());
		exit(EXIT_FAILURE);
	} else {
		SDL_Log("height is: %d ... width is: %d", tmp_h, tmp_w);
	}

	src_rect.x = 0;
	src_rect.y = 0;
	src_rect.w = 128;
	src_rect.h = 82;

	dest_rect.x = 0;
	dest_rect.y = 0;
	dest_rect.w = src_rect.w;
	dest_rect.h = src_rect.h;

	SDL_Log("init done");

	running = true;
}


void Game::render_all()
{
	int ret = SDL_RenderClear(renderer);
        if (ret != 0)
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,
			     "unable to clear renderer: %s",
			     SDL_GetError());

	ret = SDL_RenderCopyEx(renderer, texture, &src_rect, &dest_rect,
			       0, 0, SDL_FLIP_HORIZONTAL);
	if (ret != 0)
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,
			     "unable to rendercopy texture: %s",
			     SDL_GetError());

        SDL_RenderPresent(renderer);
}


void Game::cleanup_all()
{
	SDL_Log("cleanup game");
	SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
	SDL_Quit();
}


void Game::handle_events()
{
	SDL_Event e;
	if (SDL_PollEvent(&e)) {
		switch(e.type) {
		case SDL_QUIT:
			running = false;
			break;
		case SDL_JOYAXISMOTION:
			/* do something */
			break;
		case SDL_JOYBUTTONDOWN:
                        /* do something */
			break;
		case SDL_JOYBUTTONUP:
			/* do something */
			break;
		case SDL_KEYDOWN:
			/* do something */
			break;
		case SDL_KEYUP:
			/* do something */
			break;
		case SDL_MOUSEBUTTONDOWN:
			/* do something */
			break;
		case SDL_MOUSEBUTTONUP:
			/* do something */
			break;
		default:
			/* ignore this time */
			break;
		}
	}
}


void Game::update_all()
{
	src_rect.x = 128 * int(((SDL_GetTicks() / 100) % 6));
}

