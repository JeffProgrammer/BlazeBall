#version 330 core

in vec2 UV;

out vec4 color;

uniform sampler2D textureSampler;
uniform sampler2D normalSampler;
uniform sampler2D specularSampler;

void main() {
	//Texture
	vec3 materialColor = texture(textureSampler, UV).rgb;
	//vec3 specularColor = texture(specularSampler, UV).rgb;
	//vec3 normalColor = normalize(texture(normalSampler, UV).rgb * 2.0 - 1.0);

	color = vec4(materialColor, 1);
}