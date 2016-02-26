//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

uniform sampler2D inTextureSampler;
uniform sampler2D inDepthSampler;

uniform vec2 inScreenSize;
uniform vec2 inProjectionBounds;

varying vec2 outUV;

//Super simple conversion functions that make my life easier
vec2 pixelToNormal(vec2 inSize) {
	return inSize / inScreenSize;
}
vec2 normalToPixel(vec2 inSize) {
	return inSize * inScreenSize;
}

// https://www.gamedev.net/topic/562668-glsl-access-depth-buffer-from-shader/
//Convert a depth z/w component into a linear one
float linearize(float zoverw) {
	float near = inProjectionBounds.x;
	float far = inProjectionBounds.y;

	return (2.0 * near) / (far + near - zoverw * (far - near));
}

//Basic depth sampling from a texture
float depth(sampler2D sampler, vec2 uv) {
	return linearize(texture(sampler, uv).r) * 10;
}

//Easy depth that does pixeling and uses globals
float fd(float x, float y) {
	return log(depth(inDepthSampler, outUV + pixelToNormal(vec2(x, y))));
}

//Take an image and sample a 3x3 grid of pixels around a given uv. Each pixel will be multiplied
// by its corresponding value in k and then summed.
float kernel(mat3 k, sampler2D sampler, vec2 uv) {
	return (
		(fd(-1, -1) * k[0][0]) + (fd(0, -1) * k[1][0]) + (fd(1, -1) * k[2][0]) + 
		(fd(-1,  0) * k[0][1]) + (fd(0,  0) * k[1][1]) + (fd(1,  0) * k[2][1]) + 
		(fd(-1,  1) * k[0][2]) + (fd(0,  1) * k[1][2]) + (fd(1,  1) * k[2][2])
	);
}

//Toon shading, basically just round to the nearest 1/n
float toon(float x, float n) {
	return floor((x * n) + 0.5) / n;
}
//Apply toon() to a vector
vec4 toon(vec4 v, float n) {
	return vec4(toon(v.x, n), toon(v.y, n), toon(v.z, n), v.w);
}

void main() {
	//Basic texture
	gl_FragColor = texture(inTextureSampler, outUV);
	//Super cool toon shading
	gl_FragColor = toon(gl_FragColor, 8);

	//x and y Sobel kernels 
	// https://en.wikipedia.org/wiki/Sobel_operator
	mat3 kernx = mat3(-1,  0,  1,
	                  -2,  0,  2,
	                  -1,  0,  1);
	mat3 kerny = mat3(-1, -2, -1,
	                   0,  0,  0,
	                   1,  2,  1);

	//Basic Sobel operation, get derivative of depth. Gives nice black lines around edges.
	float gx = kernel(kernx, inDepthSampler, outUV);
	float gy = kernel(kerny, inDepthSampler, outUV);
	float d = 1 - sqrt(gx * gx + gy * gy);

	//Multiply by the Sobel result to overlay the lines
	gl_FragColor *= vec4(d, d, d, 1.0);
}