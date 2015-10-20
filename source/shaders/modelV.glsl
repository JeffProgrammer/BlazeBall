#version 120

attribute vec3 vertexPosition_model;
attribute vec2 vertexUV;

varying vec2 UV;

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