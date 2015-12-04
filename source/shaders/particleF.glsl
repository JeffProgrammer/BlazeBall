#version 330

in vec2 UV;
out vec4 fragColor;

uniform sampler2D textureSampler;

void main() {
	vec4 tex = texture(textureSampler, UV);
	fragColor = tex * vec4(1, 0, 0, 1);
}