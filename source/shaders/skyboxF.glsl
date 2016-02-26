//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

varying vec3 outUV;

uniform samplerCube inCubemapSampler;

void main() {
	vec3 materialColor = textureCube(inCubemapSampler, outUV).rgb;
	gl_FragColor = vec4(materialColor, 1.0);
}