//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GRAPHICS_UTIL_H_
#define _GRAPHICS_UTIL_H_

#ifndef NDEBUG
#define GRAPHICS_DEBUG
#define GL_CHECKERRORS() GL::checkErrors(__FILE__, __LINE__)
#else
#define GL_CHECKERRORS()
#endif

namespace GL {
	inline void checkErrors(const char *fileName, int lineNumber) {
		GLenum error = GL_NO_ERROR;
		while ((error = glGetError()) != GL_NO_ERROR) {
			printf("//--------------------------------------------------------\n");
			switch (error) {
				case 0x500: printf("Code: Invalid Enum\n"); break;
				case 0x501: printf("Code: Invalid Value\n"); break;
				case 0x502: printf("Code: Invalid Operation\n"); break;
				case 0x503: printf("Code: Stack Overflow\n"); break;
				case 0x504: printf("Code: Stack Underflow\n"); break;
				case 0x505: printf("Code: Out of Memory\n"); break;
				case 0x506: printf("Code: Invalid Framebuffer Operation\n"); break;
				default:    printf("Code: Unkown\n"); break;
			}
			printf("File: %s\n", fileName);
			printf("Line Number: %d\n", lineNumber);
			printf("//--------------------------------------------------------\n");
		}
	}

	inline void checkFrameBufferStatus() {
		GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
			printf("Status error: %08x\n", status);
	}
}

#endif // _GRAPHICS_UTIL_H_