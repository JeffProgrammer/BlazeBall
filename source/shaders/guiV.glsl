//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

layout(location = 0) in vec2 inVertexPosition;
layout(location = 1) in vec2 inVertexUV;
layout(location = 2) in vec4 inVertexColor;

uniform vec2 inTranslation;
uniform mat4 inProjectionMatrix;

out vec2 outUV;
out vec4 outColor;

void main() {
	gl_Position = inProjectionMatrix * vec4(inVertexPosition, 0.0f, 1.0f);
	outUV = inVertexUV;
	outColor = inVertexColor;
}