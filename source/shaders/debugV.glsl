#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;

uniform mat4 projectionMat;
uniform mat4 viewMat;

out vec3 color;

void main() {
	mat4 viewProjectionMat = projectionMat * viewMat;

	//Worldspace position
	vec4 v = vec4(vertexPosition, 1);
	gl_Position = viewProjectionMat * v;

	color = vertexColor;
}