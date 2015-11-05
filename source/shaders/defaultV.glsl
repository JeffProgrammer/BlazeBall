#version 330 core

layout(location = 0) in vec3 vertexPosition;

uniform mat4 projectionMat;
uniform mat4 modelMat;
uniform mat4 viewMat;

void main() {
	mat4 modelViewProjectionMat = projectionMat * viewMat * modelMat;

	//Worldspace position
	vec4 v = vec4(vertexPosition, 1);
	gl_Position = modelViewProjectionMat * v;
}