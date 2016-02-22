//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

varying vec2 outUV;
varying vec3 outNormal;
varying vec3 outPositionWorld;
varying vec3 outDirectionTangent;
varying vec3 outLightTangent;
varying vec3 outNormalSkybox;

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
	vec3 materialColor = texture2D(inTextureSampler, outUV).rgb;
	vec3 specularColor = texture2D(inSpecularSampler, outUV).rgb;
	vec3 normalColor = normalize(texture2D(inNormalSampler, outUV).rgb * 2.0 - 1.0);
	vec4 shade = texture2D(inNoiseSampler, floor(outUV) / 32.0);

	//Cosine of the angle from the light to the normal
	float cosTheta = clamp(dot(normalColor, outLightTangent), 0.0, 1.0);

	//Eye vector (towards the camera)
	vec3 eye = normalize(outDirectionTangent);

	//Direction that light reflects
	vec3 reflectLight = reflect(-outLightTangent, normalColor);

	//Angle from the eye vector and reflect vector
	float cosAlpha = clamp(dot(eye, reflectLight), 0.0, 1.0);

	//Diffuse
	gl_FragColor = vec4(materialColor, 1) * (inLightColor * inLightColor.a) * cosTheta;
	//Ambient
	gl_FragColor += vec4(materialColor, 1) * inAmbientColor;
	//Specular
	gl_FragColor += vec4(specularColor, 1) * (inLightColor * inLightColor.a) * pow(cosAlpha, inSpecularExponent);

	//Shaded tiles
	gl_FragColor *= shade;

	//Skybox
	vec3 directionSkybox = normalize(outPositionWorld - inCameraPos);
	vec3 reflection = reflect(directionSkybox, normalize(outNormalSkybox));

	gl_FragColor = mix(outFragColor, textureCube(inCubemapSampler, reflection), inReflectivity);
}