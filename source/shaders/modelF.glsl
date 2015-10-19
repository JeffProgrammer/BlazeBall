#version 120

varying vec2 UV;

uniform sampler2D textureSampler;
uniform sampler2D normalSampler;
uniform sampler2D specularSampler;

void main() {
	vec3 materialColor = texture2D(textureSampler, UV).rgb;
	gl_FragColor = vec4(materialColor, 1.0);
}