//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

in vec2 outUV;
in vec4 outColor;

out vec4 outFragColor;

uniform sampler2D inTextureSampler;
uniform int inHasTexture;

void main() {
	outFragColor = outColor;
	if (inHasTexture == 1)
		outFragColor = texture(inTextureSampler, outUV);
}