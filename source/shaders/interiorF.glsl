#version 330 core

in vec2 UV;
in vec3 normal;

in vec3 position_world;
in vec3 direction_camera;
in vec3 light_camera;
in vec3 normal_camera;
in vec3 direction_tangent;
in vec3 light_tangent;

in vec3 tangent_camera;
in vec3 bitangent_camera;

out vec3 color;

uniform sampler2D textureSampler;
uniform sampler2D normalSampler;
uniform sampler2D specularSampler;
uniform sampler2D noiseSampler;

uniform vec4 lightColor;
uniform vec4 ambientColor;
uniform vec3 sunPosition;
uniform float specularExponent;

void main() {
	//Renders verts as their normals like a normal map
	//color = vec3(0.5, 0.5, 0.5) + (normal * 0.5);

	//Texture
	vec3 materialColor = texture(textureSampler, UV).rgb;
	vec3 specularColor = texture(specularSampler, UV).rgb;
	vec3 normalColor = normalize(texture(normalSampler, UV).rgb * 2.0 - 1.0);

	//Cool MBU-style noise effect
	vec3 shade = texture(noiseSampler, floor(UV) / 32.0).rgb;

	//Lighting
	vec3 n = normalize(normalColor);
	vec3 l = normalize(light_tangent);

	//Cosine of the angle from the light to the normal
	float cosTheta = clamp(dot(n, l), 0, 1.0);

	//Eye vector (towards the camera)
	vec3 eye = normalize(direction_tangent);

	//Direction that light reflects
	vec3 reflect = reflect(-l, n);

	//Angle from the eye vector and reflect vector
	float cosAlpha = clamp(dot(eye, reflect), 0, 1.0);
	

	//Diffuse
	color = materialColor * (lightColor.rgb * lightColor.a) * cosTheta;
	//Ambient
	color += materialColor * ambientColor.rgb;
	//Specular
	color += specularColor * (lightColor.rgb * lightColor.a) * pow(cosAlpha, specularExponent);

	//Shade
	color *= shade;

//	color = normal_camera;

//	color = floor(color);

//	color = (normalize(light_tangent) + 1.0) / 2.0;
//	color = (normalColor + 1.0) / 2.0;
//	color = direction_tangent;
}