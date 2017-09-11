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

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define PROGNAME "sprite sheet experiments (use bmp)"
#define MY_BMP "animate.bmp"

SDL_Window *window;
SDL_Renderer *renderer;

SDL_Texture *bitmap_texture;
SDL_Rect source_rectangle;
SDL_Rect destination_rectangle;

SDL_RendererFlip flip = SDL_FLIP_NONE;

/* the global running variable -> true still running, false quit */
bool running = false;

/* change some attributes of the rectangle via mouse click */
unsigned char change_rec_attr = 0;
#define NUM_REC_CHANGES 6

#define RENDER_ALL() do {			\
		render_window();		\
	} while (0)

#define HANDLE_EVENTS() do {			\
		handle_events();		\
	} while (0)

#define UPDATE_ALL() do {			\
		change_rectangles();		\
	} while (0)


/*
 * setup main window and renderer
 */
int
init_main_window()
{
	int err = SDL_Init(SDL_INIT_EVERYTHING);
	if (err < 0) {
		fprintf(stderr, "could not init SDL (%s)\n",
			SDL_GetError());
		return -1;
	}

	window = SDL_CreateWindow(PROGNAME,
				SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				1024, 768,
				SDL_WINDOW_SHOWN);
	if (window == NULL) {
		fprintf(stderr, "could not create main window (%s)\n",
			SDL_GetError());
		return -1;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == NULL) {
		fprintf(stderr, "could not create renderer (%s)\n",
			SDL_GetError());
		return -1;
	}

	/* set color R,G,B and alpha -> in this case draw a black background */
	err = SDL_SetRenderDrawColor(renderer, 0, 150, 0, 255);
	if (err < 0) {
		fprintf(stderr, "could not set color (%s)\n",
			SDL_GetError());
		return -1;
	}

	/* only temporary */
	SDL_Surface *tmp_surface = SDL_LoadBMP(MY_BMP);
	if (tmp_surface == NULL) {
		fprintf(stderr, "could not load bitmap %s (%s)\n",
			MY_BMP, SDL_GetError());
		return -1;
	}

	bitmap_texture = SDL_CreateTextureFromSurface(renderer, tmp_surface);
	if (bitmap_texture == NULL) {
		fprintf(stderr, "could not create texture from bitmap (%s)\n",
			SDL_GetError());
		return -1;
	}

	/* not needed anymore */
	SDL_FreeSurface(tmp_surface);

	/* base position is upper left corner and full size */
	destination_rectangle.x = source_rectangle.x = 0;
	destination_rectangle.y = source_rectangle.y = 0;
	destination_rectangle.w = source_rectangle.w = 128;
	destination_rectangle.h = source_rectangle.h = 82;

	return 0;
}

/*
 * render all parts
 *
 * note: dont react on errors -> only print them
 */
void
render_window()
{
        /* clear rendering target to the drawing color */
	int err = SDL_RenderClear(renderer);
	if (err < 0)
		fprintf(stderr, "could not set clear rendering target (%s)\n",
			SDL_GetError());

	err = SDL_RenderCopyEx(renderer, bitmap_texture,
			&source_rectangle, &destination_rectangle, 0, 0, flip);

	if (err < 0)
		fprintf(stderr, "could not set render texture (%s)\n",
			SDL_GetError());

	/* bring everthing to the window -> now we see the changes */
	SDL_RenderPresent(renderer);
}

/*
 * cleanup all non system related parts -> SDL and co
 */
void
cleanup_non_system(void)
{
	printf("cleanup all SDL related stuff\n");

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

/*
 *  handle all supported events
 */
void
handle_events()
{
	SDL_Event e;
	if (SDL_PollEvent(&e)) {
		switch(e.type) {
		case SDL_QUIT:
			printf("an actual SDL_QUIT event occured\n");
			running = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			printf("an actual SDL_MouseButtonEvent event occured\n");
			if (change_rec_attr < NUM_REC_CHANGES)
				change_rec_attr++;
			else
				change_rec_attr = 0;
			printf("value of change_rec_attr = %d\n", change_rec_attr);
			break;
		default:
			printf("an actual unsupported event occured %d\n",
				e.type);
			break;
		}
	}
}

/*
 *  change the rectangle attributes
 */
void
change_rectangles()
{
	switch(change_rec_attr) {
	case 0:
		source_rectangle.x = 0;
		break;
	case 1:
		source_rectangle.x = 128;
		break;
	case 2:
		source_rectangle.x = 128 * 2;
		break;
	case 3:
		source_rectangle.x = 128 * 3;
		break;
	case 4:
		source_rectangle.x = 128 * 4;
		break;
	case 5:
		source_rectangle.x = 128 * 5;
		break;
	default:
		/* roughly 100ms */
		source_rectangle.x = 128 * (((SDL_GetTicks() / 100) % 6));
	}
}


/*
 * -----------------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{
	printf("start to try to build a SDL window\n");

	printf("usage: ./sprite_sheet [-hv]  \n");
	printf("       -h -> flip horizontal \n");
	printf("       -v -> flip vertical   \n");

	int c;
	while ((c = getopt(argc, argv, "hv")) != -1) {
		switch (c) {
		case 'h':
			flip |= SDL_FLIP_HORIZONTAL;
			printf("flip horizontal\n");
			break;
		case 'v':
			flip |= SDL_FLIP_VERTICAL;
			printf("flip vertical\n");
			break;
		default:
			fprintf(stderr, "no valid option\n");
			exit(EXIT_FAILURE);
		}
	}

	if (init_main_window() == -1) {
		fprintf(stderr, "could not setup a SDL window\n");
		exit(EXIT_FAILURE);
	} else {
		running = true;
	}

	while (running) {
		HANDLE_EVENTS();
		UPDATE_ALL();
		RENDER_ALL();
	}

	cleanup_non_system();
	SDL_Quit();

	exit(EXIT_SUCCESS);
}
