#version 120

varying vec2 UV;

uniform sampler2D textureSampler;

void main() {
	gl_FragColor = vec4(1,0,0,1); //texture2D(textureSampler, UV) * vec4(0.0, 1.0, 0.0, 1.0);
}