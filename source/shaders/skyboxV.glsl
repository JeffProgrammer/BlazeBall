#version 330 core

//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

layout(location = 0) in vec3 inVertexPosition;

out vec3 outUV;

uniform mat4 inProjectionMat;
uniform mat4 inViewMat;
uniform vec3 inCameraPos;
uniform float inExtent;

void main() {
	mat4 viewProjectionMat = inProjectionMat * inViewMat;

	//Worldspace position
	vec4 v = viewProjectionMat * vec4(inVertexPosition - (inCameraPos / inExtent), 1);

	//Note: xyww so the depth test works correctly
	gl_Position = v.xyww;

	//UVs are the same as the vertex position because it's a 1x1x1 cube
	outUV = inVertexPosition;
}