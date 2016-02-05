#version 330 core

//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

in vec3 UV;
out vec4 fragColor;

uniform samplerCube cubemapSampler;

void main() {
	vec3 materialColor = texture(cubemapSampler, UV).rgb;
	fragColor = vec4(materialColor, 1.0);
}