//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

varying vec2 outUV;

uniform sampler2D inTextureSampler;

void main() {
	vec3 materialColor = texture2D(inTextureSampler, outUV).rgb;
	gl_FragColor = vec4(materialColor, 1.0);
}