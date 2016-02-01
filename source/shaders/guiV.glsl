#version 330 core

layout(location = 0) in vec2 vertexPosition;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec4 vertexColor;

uniform vec2 translation;
uniform mat4 projectionMatrix;

out vec2 uv;
out vec4 color;

void main() {
	gl_Position = projectionMatrix * vec4(vertexPosition, 0.0f, 1.0f);
	uv = vertexUV;
	color = vertexColor;
}