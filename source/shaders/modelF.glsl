//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

in vec2 outUV;
out vec4 outFragColor;

uniform sampler2D inTextureSampler;

void main() {
	vec3 materialColor = texture(inTextureSampler, outUV).rgb;
	outFragColor = vec4(materialColor, 1.0);
}