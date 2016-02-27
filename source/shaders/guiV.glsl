//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

attribute vec2 inVertexPosition;
attribute vec2 inVertexUV;
attribute vec4 inVertexColor;

uniform vec2 inTranslation;
uniform mat4 inProjectionMatrix;

varying vec2 outUV;
varying vec4 outColor;

void main() {
	gl_Position = inProjectionMatrix * vec4(inVertexPosition, 0.0, 1.0);
	outUV = inVertexUV;
	outColor = inVertexColor;
}