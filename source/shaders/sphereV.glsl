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

attribute vec3 inVertexPosition;
attribute vec2 inVertexUV;
attribute vec3 inVertexNormal;
attribute vec3 inVertexTangent;
attribute vec3 inVertexBitangent;

varying vec2 outUV;
varying vec3 outNormal;
varying vec3 outPositionWorld;
varying vec3 outDirectionTangent;
varying vec3 outLightTangent;
varying vec3 outNormalSkybox;

uniform mat4 inProjectionMat;
uniform mat4 inViewMat;
uniform mat4 inModelMat;
uniform mat4 inInverseModelMat;
uniform vec3 inSunDirection;

void main() {
	mat4 modelViewProjectionMat = inProjectionMat * inViewMat * inModelMat;

	//Cameraspace of the vertex position and direction from vertex to camera (backwards but correct)
	vec3 positionCamera = (inViewMat * inModelMat * vec4(inVertexPosition, 1)).xyz;
	vec3 directionCamera = vec3(0, 0, 0) - positionCamera;

	//Vector from vertex to light, but in cameraspace this time
	vec3 lightCamera = -(inViewMat * vec4(inSunDirection, 0)).xyz;

	mat3 modelView3Mat = mat3(inViewMat * inModelMat);

	//The normal, tangent, bitangent, in cameraspace
	vec3 normalCamera    = modelView3Mat * inVertexNormal;
	vec3 tangentCamera   = modelView3Mat * inVertexTangent;
	vec3 bitangentCamera = modelView3Mat * inVertexBitangent;

	//Also normal mapping
	mat3 tbn = transpose(mat3(tangentCamera, bitangentCamera, normalCamera));

	outLightTangent = normalize(tbn * lightCamera);
	outDirectionTangent = tbn * directionCamera;

	//Worldspace position
	gl_Position = modelViewProjectionMat * vec4(inVertexPosition, 1);

	//Skybox information
	outNormalSkybox = mat3(transpose(inInverseModelMat)) * inVertexNormal;

	//Send to fragment shader
	outUV = inVertexUV;
	outNormal = inVertexNormal;

	//Worldspace position
	outPositionWorld = (inModelMat * vec4(inVertexPosition, 1)).xyz;
}