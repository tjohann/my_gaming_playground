some hints to remember
======================

error handling
--------------

Every SDL functions returns 0 on success or an negative error code (like pthread lib). So the correct check is

	int err = SDL_...
	if (err < 0)
		fprintf(...., "%s", SDL_GetError());


SDL_GetError() returns a string with an error string (like strerror).


rectangles
----------

SDL_RenderCopy(renderer, texture, 0, 0); -> use the entire renderer for the texture


joysticks
---------

SDL_PollEvent(&e);

	use e.jaxis.which to get the number of the joystick

	SDL_JOYAXISMOTION of: 1
	top
	SDL_JOYAXISMOTION of: 1
	middle
	SDL_JOYAXISMOTION of: 0
	top
	SDL_JOYAXISMOTION of: 0
	middle

You get 2 events from every axis movement -> the direction like -32767/left and 0/middle after release.

