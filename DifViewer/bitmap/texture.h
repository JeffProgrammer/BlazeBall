//------------------------------------------------------------------------------
// Copyright (c) 2014 Glenn Smith
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of the project nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
