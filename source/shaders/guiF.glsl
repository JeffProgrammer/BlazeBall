#version 330 core

//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

in vec2 uv;
in vec4 color;

out vec4 fragColor;

uniform sampler2D textureSampler;
uniform int hasTexture;

void main() {
	vec4 finalColor = color;
	if (hasTexture == 1)
		finalColor = texture(textureSampler, uv);
	fragColor = finalColor;
}