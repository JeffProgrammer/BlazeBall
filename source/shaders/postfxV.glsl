//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

attribute vec3 inVertexPosition;

varying vec2 outUV;

void main() {
	//Worldspace position
	gl_Position = vec4(inVertexPosition, 1);

	outUV = (inVertexPosition.xy + vec2(1, 1)) / 2.0;
}