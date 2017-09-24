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

#include "libgame.h"
#include "libgame_private.h"


LIGGAME_EXPORT int
open_config(char *file, char *name, config_t *cfg)
{
	config_init(cfg);

	if (!config_read_file(cfg, file)) {
		eprintf("could not read config file %s:%d - %s\n",
			config_error_file(cfg),
			config_error_line(cfg),
			config_error_text(cfg));
		return -1;
	}

	const char *str;
	if (!config_lookup_string(cfg, "name", &str)) {
		eprintf("no valid naming in configuration file\n");
		return -1;
	}

	if (strlen(str) != strlen(name)) {
		eprintf("not a valid configuration\n");
		return -1;
	}

	if (strncmp(str, name, strlen(name)) != 0) {
		eprintf("not a valid configuration\n");
		return -1;
	}

	return 0;
}
