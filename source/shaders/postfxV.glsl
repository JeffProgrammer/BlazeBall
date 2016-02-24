#version 330 core

//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

layout(location = 0) in vec3 inVertexPosition;

out vec2 outUV;

void main() {
	//Worldspace position
	gl_Position = vec4(inVertexPosition, 1);

	outUV = (inVertexPosition.xy + vec2(1, 1)) / 2.0;
}