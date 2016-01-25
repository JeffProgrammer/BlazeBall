//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GRAPHICS_UTIL_H_
#define _GRAPHICS_UTIL_H_

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

#endif // _GRAPHICS_UTIL_H_