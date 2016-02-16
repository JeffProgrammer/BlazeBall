#version 330 core

//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

in vec2 UV;
in vec3 normal;

in vec3 position_world;
in vec3 direction_camera;
in vec3 light_camera;
in vec3 normal_camera;
in vec3 direction_tangent;
in vec3 light_tangent;
in vec3 normal_skybox;

in vec3 tangent_camera;
in vec3 bitangent_camera;

out vec4 fragColor;

uniform sampler2D textureSampler;
uniform sampler2D normalSampler;
uniform sampler2D specularSampler;
uniform samplerCube cubemapSampler;

uniform mat4 inverseRotMat;

uniform vec4 lightColor;
uniform vec4 ambientColor;
uniform vec3 sunPosition;
uniform float specularExponent;
uniform vec3 cameraPos;
uniform float reflectivity;

void main() {
	//Texture
	vec3 materialColor = texture(textureSampler, UV).rgb;
	vec3 specularColor = texture(specularSampler, UV).rgb;
	vec3 normalColor = normalize(texture(normalSampler, UV).rgb * 2.0 - 1.0);


	//Cosine of the angle from the light to the normal
	float cosTheta = clamp(dot(normalColor, light_tangent), 0.0, 1.0);

	//Eye vector (towards the camera)
	vec3 eye = normalize(direction_tangent);

	//Direction that light reflects
	vec3 reflect_light = reflect(-light_tangent, normalColor);

	//Angle from the eye vector and reflect vector
	float cosAlpha = clamp(dot(eye, reflect_light), 0.0, 1.0);


	//Diffuse
	vec3 color = materialColor * (lightColor.rgb * lightColor.a) * cosTheta;
	//Ambient
	color += materialColor * ambientColor.rgb;
	//Specular
	color += specularColor * (lightColor.rgb * lightColor.a) * pow(cosAlpha, specularExponent);

	fragColor = vec4(color, 1);

	//Skybox
	vec3 direction_skybox = normalize(position_world - cameraPos);
	vec3 reflection = reflect(direction_skybox, normalize(normal_skybox));

	fragColor = mix(fragColor, texture(cubemapSampler, reflection), reflectivity);
}