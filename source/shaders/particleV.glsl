#version 120

attribute vec2 vertex;
attribute vec3 position;
attribute vec3 velocity;
attribute float creation;

varying vec2 UV;

uniform mat4 projectionMat;
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 inverseCameraMatrix;
uniform float currentTime;

void main() {
	mat4 modelViewProjectionMat = projectionMat * viewMat * modelMat;

	float elapsed = currentTime - creation;
	vec3 velOff = velocity * elapsed;

	//Take the model and rotate it to face the camera. Since position is multiplied
	// by camera, we need to undo that when we get the model position.
	vec3 modelVert = (inverseCameraMatrix * vec4(vertex, 0, 0)).xyz;

	//World space position just uses particle position and model position
	vec3 pos = position + modelVert + velOff;
	gl_Position = modelViewProjectionMat * vec4(pos, 1);

	//Send to fragment shader, particle UV is offset by 0.5 because verts are
	// (-0.5, -0.5) -> (0.5, 0.5)
	UV = vertex + vec2(0.5);
}