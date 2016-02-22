//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

out vec4 fragColor;
in vec3 color;

void main() {
	fragColor = vec4(color, 1);
}