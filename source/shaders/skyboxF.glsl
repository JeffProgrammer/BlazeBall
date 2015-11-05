#version 330 core

in vec3 UV;
out vec4 fragColor;

uniform samplerCube cubemapSampler;

void main() {
	vec3 materialColor = texture(cubemapSampler, UV).rgb;
	fragColor = vec4(materialColor, 1.0);
}