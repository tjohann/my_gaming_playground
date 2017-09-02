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


Examples
--------

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
