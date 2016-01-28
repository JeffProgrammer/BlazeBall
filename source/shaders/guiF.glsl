#version 330 core

in vec2 uv;
in vec4 color;

out vec4 fragColor;

uniform sampler2D textureSampler;
uniform int hasTexture;

void main() {
	vec4 finalColor = color;
	if (hasTexture)
		finalColor += texture(textureSampler.st, uv);
	fragColor = finalColor;
}