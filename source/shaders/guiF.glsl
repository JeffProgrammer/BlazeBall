//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

varying vec2 outUV;
varying vec4 outColor;

uniform sampler2D inTextureSampler;
uniform int inHasTexture;

void main() {
	// Check to make sure that we have a texture that we are rendering.
	// If not, just output the color to the fragment.
	if (inHasTexture == 1)
		gl_FragColor = texture(inTextureSampler, outUV) * outColor;
	else
		gl_FragColor = outColor;
}