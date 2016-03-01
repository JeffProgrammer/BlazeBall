//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

varying vec3 outUV;

uniform int inLines;

void main() {
	if (inLines == 1)
		gl_FragColor = vec4(1, 0, 1, 1);
	else
		gl_FragColor = vec4(1, 1, 1, 0.4);
}