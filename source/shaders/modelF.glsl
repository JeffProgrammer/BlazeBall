#version 330 core

in vec2 UV;
out vec4 fragColor;

uniform sampler2D textureSampler;
uniform sampler2D normalSampler;
uniform sampler2D specularSampler;

void main() {
	vec3 materialColor = texture(textureSampler, UV).rgb;
	fragColor = vec4(materialColor, 1.0);
}