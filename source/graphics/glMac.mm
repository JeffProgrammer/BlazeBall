//-----------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeffrey Hutchinson
// All rights reserved.
//-----------------------------------------------------------------------------

#import <Cocoa/Cocoa.h>
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
		CGLDescribeRenderer(renderer, i, kCGLRPVideoMemoryMegabytes, &memory);
	}
	
	// Cleanup.
	CGLDestroyRendererInfo(renderer);
	return memory;
}