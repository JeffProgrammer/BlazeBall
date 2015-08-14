#version 330 core

layout (location = 0) in vec3 vertexPosition_model;
layout (location = 1) in vec2 vertexUV;
layout (location = 2) in vec3 vertexNormal;
layout (location = 3) in vec3 vertexTangent;
layout (location = 4) in vec3 vertexBitangent;

out vec2 UV;
out vec3 normal;

out vec3 position_world;
out vec3 direction_camera;
out vec3 light_camera;
out vec3 normal_camera;
out vec3 direction_tangent;
out vec3 light_tangent;

out vec3 tangent_camera;
out vec3 bitangent_camera;

uniform mat4 projectionMat;
uniform mat4 modelMat;
uniform mat4 viewMat;

uniform vec4 lightColor;
uniform vec4 ambientColor;
uniform vec3 sunPosition;
uniform float specularExponent;

void main() {
	mat4 modelViewProjectionMat = projectionMat * viewMat * modelMat;

	//Worldspace position
	vec4 v = vec4(vertexPosition_model, 1);
	gl_Position = modelViewProjectionMat * v;

	//Send to fragment shader
	UV = vertexUV;
	normal = vertexNormal;

	//Worldspace position
	position_world = (modelMat * vec4(vertexPosition_model, 1)).xyz;

	//Cameraspace of the vertex position and direction from vertex to camera (backwards but correct)
	vec3 position_camera = (viewMat * modelMat * vec4(vertexPosition_model, 1)).xyz;
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
}