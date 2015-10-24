#version 120

attribute vec3 vertexPosition;
attribute vec2 vertexUV;
attribute vec3 vertexNormal;
attribute vec3 vertexTangent;
attribute vec3 vertexBitangent;

varying vec2 UV;
varying vec3 normal;

varying vec3 position_world;
varying vec3 direction_camera;
varying vec3 light_camera;
varying vec3 normal_camera;
varying vec3 direction_tangent;
varying vec3 light_tangent;
varying vec3 normal_skybox;
varying vec3 position_skybox;

varying vec3 tangent_camera;
varying vec3 bitangent_camera;

uniform mat4 projectionMat;
uniform mat4 modelMat;
uniform mat4 viewMat;
//uniform mat4 skyboxMat;
uniform mat4 inverseModelMat;

uniform vec4 lightColor;
uniform vec4 ambientColor;
uniform vec3 sunPosition;
uniform float specularExponent;
uniform vec3 cameraPos;

void main() {
	mat4 modelViewProjectionMat = projectionMat * viewMat * modelMat;

	//Worldspace position
	vec4 v = vec4(vertexPosition, 1);
	gl_Position = modelViewProjectionMat * v;

	//Send to fragment shader
	UV = vertexUV;
	normal = vertexNormal;

	//Worldspace position
	position_world = (modelMat * vec4(vertexPosition, 1)).xyz;

	//Cameraspace of the vertex position and direction from vertex to camera (backwards but correct)
	vec3 position_camera = (viewMat * modelMat * vec4(vertexPosition, 1)).xyz;
	direction_camera = vec3(0, 0, 0) - position_camera;

	//Vector from vertex to light, but in cameraspace this time
	vec3 lightPos_camera = (viewMat * vec4(sunPosition, 1)).xyz;
	light_camera = lightPos_camera + direction_camera;

	mat3 modelView3Mat = mat3(viewMat * modelMat);

	//The normal, tangent, bitangent, in cameraspace
	normal_camera = modelView3Mat * vertexNormal;
	tangent_camera = modelView3Mat * vertexTangent;
	bitangent_camera = modelView3Mat * vertexBitangent;

	//Also normal mapping
	mat3 tbn = transpose(mat3(tangent_camera, bitangent_camera, normal_camera));

	light_tangent = tbn * light_camera;
	direction_tangent = tbn * direction_camera;

	//Skybox information
	normal_skybox = mat3(transpose(inverseModelMat)) * vertexNormal;
}