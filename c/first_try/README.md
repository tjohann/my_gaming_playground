FIRST - TRY
===========

My first experiments


Overview of the code
--------------------

To build all type

	make

To run a menu from where you can start all examples type

	make run

after building all.


Library
-------

libgame:

	- some useful functions used in some examples

The library implements 2 different models of an game object (see examples below for usage)

	- simple form wich uses only data -> game_obj_data_t
	- a more complex form wich uses data and functions pointers -> game_obj_t


Examples (stand alone)
----------------------

Note: the order of the examples represents something like an complexity gradient.

hello_sdl:

	- simple SDL example to check if everthing is installed correctly

	usage: ./hello_sdl [-f]
	       -f -> fullscreen

first_window:

	- simple SDL example with something like a skeleton for base game functionallity

rectangle:

	- simple SDL example which show the usage of source and target rectangles with an small rider bitmap
	- to change the behaviour, click a mouse button

sprite_sheet:

	- simple SDL example which show the usage of sprites with a small cat sprite sheet

	usage: ./sprite_sheet [-hv]
           -h -> flip horizontal
           -v -> flip vertical

sprite_sheet_2:

	- same example like sprite_sheet, but uses a png instead of a bmp (alpha topic)


Examples (using libgame with simple game object)
------------------------------------------------

use_game_obj_t:

	- use game_obj_t as an object representation and let some pixmaps run over the screen

input_control:

	- use joystick or keyboard (cursor or WASD) to move an object over the screen (velocity resets after release of button or middle position of joystick)
	- first example which should also work on our tv (1280x720 size and an astronaut of 50x60)

follow_mouse:

	- follow the position of a mouse click (only 50% of click position)

flying_astronaut:

	- use joystick or keyboard to control the direction of the movement (velocity wont reset after release of button or middle position of joystick)
	- the screen is like a cage with an flying ball/astronaut
	- use + and - to inc/dec velocity (x and y!)


Examples (using libgame with complex game object)
-------------------------------------------------

object_fab:

	- similar to flying_astronaut but using the object fab to generate all objects taken from config file

joystick_fab:

	- similar to object_fab with joystick handling via config file

simple_ai (state: open):

	- similiar to the joystick_fab, but with moving ememies


Games
-----

simple_tennis:

	- something like a pong clone (https://en.wikipedia.org/wiki/Pong)
