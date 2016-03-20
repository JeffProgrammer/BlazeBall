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

#import <Cocoa/Cocoa.h>
#import <availability.h>
#include "graphics/glMac.h"

// Reference: https://developer.apple.com/library/mac/qa/qa1168/_index.html
GLint getVideoMemoryAPPLE(void *context) {
	GLint memory = 0;
	
	id contextObj = (id)((CGLContextObj)context);
	GLint virtualScreen = [contextObj currentVirtualScreen];
	
	// Grab the current display mask and the renderID that will identify whih device
	// that we are querying.
	GLint displayMask;
	GLint renderId;
	[[contextObj pixelFormat] getValues:&displayMask forAttribute:NSOpenGLPFAScreenMask forVirtualScreen:virtualScreen];
	[[contextObj pixelFormat] getValues:&renderId forAttribute:NSOpenGLPFARendererID forVirtualScreen:virtualScreen];
	
	// Grab our renderers and loop through to find out which one is ours.
	GLint numRenderers;
	CGLRendererInfoObj renderer;
	CGLQueryRendererInfo(displayMask, &renderer, &numRenderers);
	
	// find our renderer
	for (GLint i = 0; i < numRenderers; i++) {
		GLint checkRenderId;
		CGLDescribeRenderer(renderer, i, kCGLRPRendererID, &checkRenderId);
		
		// Make sure this is the one we want
		if (checkRenderId != renderId)
			continue;
		
		// Grab it
#if __MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
		CGLDescribeRenderer(renderer, i, kCGLRPVideoMemoryMegabytes, &memory);
#else
		CGLDescribeRenderer(renderer, i, kCGLRPVideoMemory, &memory);
		memory /= 1024 * 1024; // bytes -> megabytes
#endif
	}
	
	// Cleanup.
	CGLDestroyRendererInfo(renderer);
	return memory;
}