#version 120

attribute vec2 vertex;
attribute vec3 position;

varying vec2 UV;

uniform mat4 projectionMat;
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform vec3 cameraRight;
uniform vec3 cameraUp;

void main() {
	mat4 modelViewProjectionMat = projectionMat * viewMat * modelMat;

	// world space position
	vec3 pos = position + cameraRight * vertex.x + cameraUp * vertex.y;
	vec4 v = vec4(pos, 1);
	gl_Position = modelViewProjectionMat * v;

	//Send to fragment shader
	UV = vertex * 2.0;
}