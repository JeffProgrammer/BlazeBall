//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the project nor the
//      names of its contributors may be used to endorse or promote products
//      derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

	gl_FragColor = mix(gl_FragColor, textureCube(inCubemapSampler, reflection), inReflectivity);
}