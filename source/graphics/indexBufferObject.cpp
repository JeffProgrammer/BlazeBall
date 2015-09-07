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

#include "graphics/indexBufferObject.h"

IndexBufferObject::IndexBufferObject() {
	mIBO = 0;
	mBufferType = BufferType::NONE;
}

IndexBufferObject::~IndexBufferObject() {
	assert(glIsBuffer(mIBO) == false);
	glDeleteBuffers(1, &mIBO);
}

void IndexBufferObject::bind() {
	assert(mIBO != 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
}

void IndexBufferObject::unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBufferObject::submit(const U16 *data, const U16 count) {
	glGenBuffers(1, &mIBO);
	
	// bind this buffer
	bind();
	
	GLenum bufferType = GLUtils::convertBufferType(mBufferType);
	assert(bufferType != GL_ZERO);
	
	// upload to the gpu
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(U16) * count, data, bufferType);
	
	// finished uploading data to the GL
	unbind();
}

void IndexBufferObject::setBufferType(BufferType type) {
	mBufferType = type;
}