#version 330 core

in vec2 UV;
in vec3 normal;
in vec3 light;
out vec3 color;

uniform sampler2D textureSampler;
uniform sampler2D noiseSampler;

void main() {
	color = texture(textureSampler, UV).rgb;
	vec3 shade = texture(noiseSampler, floor(UV) / 32.0).rgb;
	color.r *= 0.5 + shade.r;
	color.g *= 0.5 + shade.g;
	color.b *= 0.5 + shade.b;
}