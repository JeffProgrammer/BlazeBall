#version 120

varying vec3 UV;

uniform samplerCube cubemapSampler;

void main() {
	vec3 materialColor = textureCube(cubemapSampler, UV).rgb;
	gl_FragColor = vec4(materialColor, 1.0);
}