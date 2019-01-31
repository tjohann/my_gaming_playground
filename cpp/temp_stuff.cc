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
