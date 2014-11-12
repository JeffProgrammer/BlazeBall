//------------------------------------------------------------------------------
// Copyright (C) 2014 Glenn Smith
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//------------------------------------------------------------------------------

#ifndef texture_h
#define texture_h

#include <OpenGL/gl.h>
#include <stdbool.h>
#include "types.h"

typedef struct {
	bool generated;
	GLuint buffer;
	Point2I extent;
	U8 *pixels;
} Texture;

Texture *texture_create_from_pixels(U8 *pixels, Point2I extent);
void texture_release(Texture *texture);

void texture_generate_buffer(Texture *texture);
void texture_activate(Texture *texture);
void texture_deactivate(Texture *texture);

#endif
