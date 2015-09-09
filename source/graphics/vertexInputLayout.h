//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
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

#ifndef _GRAPHICS_VERTEXINPUTLAYOUT_H_
#define _GRAPHICS_VERTEXINPUTLAYOUT_H_

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif // __APPLE__

#include "base/types.h"
#include <string>
#include <vector>

struct VertexInput {
	U32 attribNumber;
	U32 size;
	U32 type;
	size_t stride;
	void *offset;

	VertexInput(U32 attribNum, U32 size, U32 type, size_t stride, void *offset) {
		this->attribNumber = attribNum;
		this->size = size;
		this->type = type;
		this->stride = stride;
		this->offset = offset;
	}
};

/**
 * This class defines a layout for a shader's attributes. It is based off of 
 * the OpenGL attrib layout system. After creating the VertexInputLayout, fill
 * up the layout by calling set with each VertexInput. Then, you can bind and
 * unbind for each shader change.
 */
class VertexInputLayout {
private:
	/**
	 * The vertex input layout, representing each layout line, in order.
	 */
	std::vector<VertexInput> mVertexInputs;

public:
	VertexInputLayout();
	~VertexInputLayout();

	/**
	 * Binds the vertex input layout into the graphics API.
	 */
	void bind();

	/**
	 * Unbinds the vertex input layout into the graphics API.
	 */
	void unbind();

	/**
	 * Appends a line of vertex attribute layout into the internal structure 
	 * holding the layout.
	 * @param [in] input The vertex input for the specific attribute.
	 */
	void set(const VertexInput &input);
};

#endif // _GRAPHICS_VERTEXINPUTLAYOUT_H_