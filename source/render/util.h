//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the project nor the
//      names of its contributors may be used to endorse or promote products
//      derived from this software without specific prior written permission.
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

#ifndef _RENDER_UTIL_H_
#define _RENDER_UTIL_H_

#include "base/io.h"

#ifndef NDEBUG
#define GRAPHICS_DEBUG
#define GL_CHECKERRORS() GLL::checkErrors(__FILE__, __LINE__)
#else
#define GL_CHECKERRORS()
#endif

namespace GLL {
	inline void checkErrors(const char *fileName, int lineNumber) {
		GLenum error = GL_NO_ERROR;
		while ((error = glGetError()) != GL_NO_ERROR) {
			IO::printf("//--------------------------------------------------------\n");
			IO::printf("OpenGL Error:\n");
			switch (error) {
				case 0x500: IO::printf("Code: Invalid Enum\n"); break;
				case 0x501: IO::printf("Code: Invalid Value\n"); break;
				case 0x502: IO::printf("Code: Invalid Operation\n"); break;
				case 0x503: IO::printf("Code: Stack Overflow\n"); break;
				case 0x504: IO::printf("Code: Stack Underflow\n"); break;
				case 0x505: IO::printf("Code: Out of Memory\n"); break;
				case 0x506: IO::printf("Code: Invalid Framebuffer Operation\n"); break;
				default:    IO::printf("Code: Unkown\n\n"); break;
			}
			IO::printf("File: %s\n", fileName);
			IO::printf("Line Number: %d\n", lineNumber);
			IO::printf("//--------------------------------------------------------\n");
		}
	}

	inline void checkFrameBufferStatus() {
		GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
			IO::printf("Status error: %08x\n", status);
	}
}

#endif // _RENDER_UTIL_H_