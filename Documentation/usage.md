some documenation of the intended usage
=======================================

The library (libgame) is the fundament for something like a gaming framework. It should include everthing to build-up simple retro/acarde games like pitfall or so.

Everthing starts with the game-loop:

	/* init done */
	running = true;

	uint32_t frame_start, frame_time;
	unsigned char local_fps =  FPS + 1; /* the first run never fits */
	uint32_t delay_time = 1000.0f / local_fps;

	while (running) {
		frame_start = SDL_GetTicks();

		HANDLE_EVENTS();
		UPDATE_ALL();
		RENDER_ALL();

		frame_time = SDL_GetTicks() - frame_start;

		if (frame_time < delay_time) {
			SDL_Delay(delay_time - frame_time);
		} else {
			printf("underrun -> try to reduce actual FPS %d\n",
				local_fps);
			local_fps--;
			delay_time = 1000.0f / local_fps;
		}
	}

It's build from the three topics

	- handle events
	- update all
	- render all


handle events
-------------

In the handle events part all events queued by the SDL-Event-Queue will be polled and the different inputs will be handled by simple switch-case statements.

For example (see flying_astronaut):

	if (SDL_PollEvent(&e)) {
		switch(e.type) {
		case SDL_QUIT:
			printf("quit the example -> bye %s\n", getenv("USER"));
			running = false;
			break;
		case SDL_JOYAXISMOTION:
			printf("SDL_JOYAXISMOTION of: %d\n", e.jaxis.which);
			tip_joystick_axis_move(&e, &velo, 1);
			break;
		case SDL_JOYBUTTONDOWN:
            /* do something */
			break;
		case SDL_JOYBUTTONUP:
			/* do something */
			break;
		case SDL_KEYDOWN:
			printf("SDL_KEYDOWN\n");
			tip_keyboard_cursor_move(&velo, 1);
			handle_keyboard_calc_keys(&velo, 1);
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
			printf("an actual unsupported event occured %d\n",
				e.type);
			break;
		}
	}

In this case we in/decrease the velocity of via the velo-struct.


update all
----------

In the update-all part the changes made during handle-event will be set to the corresponding objects. In this case only to the player:

	set_object_velo(player, &velo);

In a next step the borders of the screen will be checked and corrected:

	int x = get_object_pos_x(player);
	if (x > (SCREEN_WIDTH - get_object_size_w(player)) || x < 0)
		inv_vec_x(&velo);

	int y = get_object_pos_y(player);
	if (y > (SCREEN_HEIGHT - get_object_size_h(player)) || y < 0)
		inv_vec_y(&velo);


render all
----------

In the render-all part we finally draw the objects to the screen:

	/* clear rendering target to the drawing color */
	int err = SDL_RenderClear(renderer);
	if (err < 0)
		fprintf(stderr, "could not set clear rendering target (%s)\n",
			SDL_GetError());

	draw_object(player, renderer);

	/* bring everthing to the window -> now we see the changes */
	SDL_RenderPresent(renderer);


next steps
----------

At the moment everthing is more or less hardcoded. In a next step i want to use arrays with pointers to the corresponding objects (like in input_control example) based on a configuration file.

The idea is to read an xml file for the player, enemy(s) and the world. Then the object fab creates the array with the different game objects.
