some hints to remember
======================

error handling
--------------

Every SDL functions returns 0 on success or an negative error code (like pthread lib). So the correct check is

	int err = SDL_...
	if (err < 0)
		fprintf(...., "%s", SDL_GetError());


SDL_GetError() returns a string with an error string (like strerror).


