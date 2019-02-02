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

#ifndef _LOADER_PARAMS_H_
#define _LOADER_PARAMS_H_

#include <string>

class Loader_params
{
public:
	Loader_params(int x, int y, int w, int h, std::string texture_id)
		: x_(x), y_(y), w_(w), h_(h), texture_id_(texture_id)
		{ }

	int get_x() const { return x_;}
	int get_y() const { return y_;}
	int get_w() const { return w_;}
	int get_h() const { return h_;}

	std::string get_texture_id() const { return texture_id_; }

private:
	int x_;
	int y_;

	int w_;
	int h_;

	std::string texture_id_;
};


#endif
