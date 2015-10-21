#version 120

attribute vec3 vertexPosition;

varying vec3 UV;

uniform mat4 projectionMat;
uniform mat4 modelMat;
uniform mat4 viewMat;

void main() {
	mat4 modelViewProjectionMat = projectionMat * viewMat;

	//Worldspace position
	vec4 v = modelViewProjectionMat * vec4(vertexPosition, 1);
	gl_Position = v.xyww;

	//Send to fragment shader
	UV = vertexPosition;
}