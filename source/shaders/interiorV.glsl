//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

layout(location = 0) in vec3 inVertexPosition;
layout(location = 1) in vec2 inVertexUV;
layout(location = 2) in vec3 inVertexNormal;
layout(location = 3) in vec3 inVertexTangent;
layout(location = 4) in vec3 inVertexBitangent;

out vec2 outUV;
out vec3 outNormal;
out vec3 outPositionWorld;
out vec3 outDirectionTangent;
out vec3 outLightTangent;
out vec3 outNormalSkybox;

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