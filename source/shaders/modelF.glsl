#version 330 core

//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

in vec2 UV;
out vec4 fragColor;

uniform sampler2D textureSampler;
uniform sampler2D normalSampler;
uniform sampler2D specularSampler;

void main() {
	vec3 materialColor = texture(textureSampler, UV).rgb;
	fragColor = vec4(materialColor, 1.0);
}