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

/**
 Creates and allocates a Texture from a pixel array and extent
 @arg pixels - A 4-component list of pixels in a RGBA list
 @arg extent - A Point2I with the image extent
 @return A Texture that can be activated / generated
 */
Texture *texture_create_from_pixels(U8 *pixels, Point2I extent);

/**
 Releases a Texture, freeing both its store and its buffer
 @arg texture - The Texture to release
 */
void texture_release(Texture *texture);


/**
 Generates the OpenGL buffer for a Texture. Don't call this before setting
 up the OpenGL canvas!
 @arg texture - The Texture to generate buffers for
 */
void texture_generate_buffer(Texture *texture);

/**
 Activates a Texture for drawing with OpenGL and binds its buffer
 @arg texture - The Texture for activating
 */
void texture_activate(Texture *texture);

/**
 Deactivates a Texture after drawing
 @arg texture - The Texture for deactivating
 */
void texture_deactivate(Texture *texture);

#endif
