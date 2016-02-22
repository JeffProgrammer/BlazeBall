//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

attribute vec3 inVertexPosition;
attribute vec2 inVertexUV;

varying vec2 outUV;

uniform mat4 inProjectionMat;
uniform mat4 inModelMat;
uniform mat4 inViewMat;

void main() {
	mat4 modelViewProjectionMat = inProjectionMat * inViewMat * inModelMat;

	//Worldspace position
	gl_Position = modelViewProjectionMat * vec4(inVertexPosition, 1);

	//Send to fragment shader
	outUV = inVertexUV;
}