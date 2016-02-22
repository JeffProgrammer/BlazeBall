//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

in vec3 outUV;
out vec4 outFragColor;

uniform samplerCube inCubemapSampler;

void main() {
	vec3 materialColor = texture(inCubemapSampler, outUV).rgb;
	outFragColor = vec4(materialColor, 1.0);
}