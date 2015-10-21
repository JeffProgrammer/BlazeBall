#version 120

attribute vec3 vertexPosition;

varying vec3 UV;

uniform mat4 projectionMat;
uniform mat4 modelMat;
uniform mat4 viewMat;

void main() {
	mat4 viewProjectionMat = projectionMat * viewMat;

	//Worldspace position
	vec4 v = viewProjectionMat * vec4(vertexPosition, 1);

	//Note: xyww so the depth test works correctly
	gl_Position = v.xyww;

	//UVs are the same as the vertex position because it's a 1x1x1 cube
	UV = vertexPosition;
}