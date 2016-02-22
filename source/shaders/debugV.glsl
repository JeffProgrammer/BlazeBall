//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

attribute vec3 vertexPosition;
attribute vec3 vertexColor;

uniform mat4 projectionMat;
uniform mat4 viewMat;

varying vec3 color;

void main() {
	mat4 viewProjectionMat = projectionMat * viewMat;

	//Worldspace position
	vec4 v = vec4(vertexPosition, 1);
	gl_Position = viewProjectionMat * v;

	color = vertexColor;
}