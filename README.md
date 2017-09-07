my playground regarding game developement
=========================================

Something like a summary around game development which should lead in a series of retro games (https://github.com/tjohann/crasonars/blob/master/Documentation/ideas.md).

Note: the bitmaps within the examples are taken from http://www.widgetworx.com/projects/sl.html (like in the book below).


The book
--------

SDL Game Development:

	https://www.packtpub.com/game-development/sdl-game-development

Linux versions of the two games:

	https://github.com/WillSams/Conan-the-Caveman-Linux
	https://github.com/WillSams/Alien-Attack-Linux


SDL stuff
---------

API-Docu of SDL -> https://wiki.libsdl.org/CategoryAPI


Library
-------

libgame:
	- some useful functions used in some examples


Examples (stand alone)
----------------------

Note: the order of the examples represents something like an complexity gradient.

hello_sdl:

	- simple SDL example to check if everthing is installed correctly

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


Examples (using libgame)
------------------------

use_game_obj_t:

	- use game_obj_t as an object representation and let some pixmaps run over the screen

input_control:

	- use joystick, mouse and keyboard to move an object over the screen
	- first example which should also work on our tv (1280x720 size, 50 fps and astronaut of 50x60)
