#version 330 core

layout(location = 0) in vec3 vertexPosition;

out vec3 UV;

uniform mat4 projectionMat;
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform vec3 cameraPos;
uniform float extent;

void main() {
	mat4 viewProjectionMat = projectionMat * viewMat;

	//Worldspace position
	vec4 v = viewProjectionMat * vec4(vertexPosition - (cameraPos / extent), 1);

	//Note: xyww so the depth test works correctly
	gl_Position = v.xyww;

	//UVs are the same as the vertex position because it's a 1x1x1 cube
	UV = vertexPosition;
}