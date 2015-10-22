#version 120

varying vec2 UV;

uniform sampler2D textureSampler;

void main() {
	vec4 tex = texture2D(textureSampler, UV);
	gl_FragColor = tex * vec4(0.0, 0.1, 0.0, 0.5);
}