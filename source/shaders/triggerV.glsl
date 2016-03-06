//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

attribute vec3 inVertexPosition;

uniform mat4 inProjectionMat;
uniform mat4 inModelMat;
uniform mat4 inViewMat;
uniform int inLines;

varying vec3 outUV;

void main() {
	mat4 modelViewProjectionMat = inProjectionMat * inViewMat * inModelMat;

	outUV = inVertexPosition + vec3(0.5); //Vertex is [-0.5, 0.5]

	//Worldspace position
	gl_Position = modelViewProjectionMat * vec4(inVertexPosition, 1);

	if (inLines == 1)
		gl_Position += vec4(inVertexPosition * 0.0001, 0);
}