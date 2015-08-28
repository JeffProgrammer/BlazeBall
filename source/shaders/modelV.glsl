#version 330 core

layout (location = 0) in vec3 vertexPosition_model;
layout (location = 1) in vec2 vertexUV;
layout (location = 2) in vec3 vertexNormal;
layout (location = 3) in vec3 vertexTangent;
layout (location = 4) in vec3 vertexBitangent;

out vec2 UV;

uniform mat4 projectionMat;
uniform mat4 modelMat;
uniform mat4 viewMat;

void main() {
	mat4 modelViewProjectionMat = projectionMat * viewMat * modelMat;

	//Worldspace position
	vec4 v = vec4(vertexPosition_model, 1);
	gl_Position = modelViewProjectionMat * v;

	//Send to fragment shader
	UV = vertexUV;
}