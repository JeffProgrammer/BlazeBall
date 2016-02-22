//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

in vec2 outUV;
in vec3 outNormal;
in vec3 outPositionWorld;
in vec3 outDirectionTangent;
in vec3 outLightTangent;
in vec3 outNormalSkybox;

out vec4 outFragColor;

uniform sampler2D   inTextureSampler;
uniform sampler2D   inNormalSampler;
uniform sampler2D   inSpecularSampler;
uniform sampler2D   inNoiseSampler;
uniform samplerCube inCubemapSampler;

uniform vec4  inLightColor;
uniform vec4  inAmbientColor;
uniform float inSpecularExponent;
uniform vec3  inCameraPos;
uniform float inReflectivity;

void main() {
	//Texture
	vec3 materialColor = texture(inTextureSampler, outUV).rgb;
	vec3 specularColor = texture(inSpecularSampler, outUV).rgb;
	vec3 normalColor = normalize(texture(inNormalSampler, outUV).rgb * 2.0 - 1.0);
	vec4 shade = texture(inNoiseSampler, floor(outUV) / 32.0);

	//Cosine of the angle from the light to the normal
	float cosTheta = clamp(dot(normalColor, outLightTangent), 0.0, 1.0);

	//Eye vector (towards the camera)
	vec3 eye = normalize(outDirectionTangent);

	//Direction that light reflects
	vec3 reflectLight = reflect(-outLightTangent, normalColor);

	//Angle from the eye vector and reflect vector
	float cosAlpha = clamp(dot(eye, reflectLight), 0.0, 1.0);

	//Diffuse
	outFragColor = vec4(materialColor, 1) * (inLightColor * inLightColor.a) * cosTheta;
	//Ambient
	outFragColor += vec4(materialColor, 1) * inAmbientColor;
	//Specular
	outFragColor += vec4(specularColor, 1) * (inLightColor * inLightColor.a) * pow(cosAlpha, inSpecularExponent);

	//Shaded tiles
	outFragColor *= shade;

	//Skybox
	vec3 directionSkybox = normalize(outPositionWorld - inCameraPos);
	vec3 reflection = reflect(directionSkybox, normalize(outNormalSkybox));

	outFragColor = mix(outFragColor, texture(inCubemapSampler, reflection), inReflectivity);
}