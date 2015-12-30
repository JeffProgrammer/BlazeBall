//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "render/shader.h"

void Shader::initializeShaders() {
	// Interior shader
	Shader *interiorShader = new Shader("Interior", "interiorV.glsl", "interiorF.glsl");
	interiorShader->addUniformLocation("textureSampler", 0);
	interiorShader->addUniformLocation("normalSampler", 1);
	interiorShader->addUniformLocation("specularSampler", 2);
	interiorShader->addUniformLocation("noiseSampler", 3);
	interiorShader->addUniformLocation("cubemapSampler", 4);

	interiorShader->addAttribute("vertexPosition", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, point)));
	interiorShader->addAttribute("vertexUV", 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));
	interiorShader->addAttribute("vertexNormal", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
	interiorShader->addAttribute("vertexTangent", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tangent)));
	interiorShader->addAttribute("vertexBitangent", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, bitangent)));

	// Sphere shader
	Shader *sphereShader = new Shader("Sphere", "sphereV.glsl", "sphereF.glsl");
	sphereShader->addUniformLocation("textureSampler", 0);
	sphereShader->addUniformLocation("normalSampler", 1);
	sphereShader->addUniformLocation("specularSampler", 2);
	sphereShader->addUniformLocation("cubemapSampler", 3);
	sphereShader->addAttribute("vertexPosition", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, point)));
	sphereShader->addAttribute("vertexUV", 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));
	sphereShader->addAttribute("vertexNormal", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
	sphereShader->addAttribute("vertexTangent", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tangent)));
	sphereShader->addAttribute("vertexBitangent", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, bitangent)));

	// Skybox shader
	Shader *skyboxShader = new Shader("Skybox", "skyboxV.glsl", "skyboxF.glsl");
	skyboxShader->addUniformLocation("cubemapSampler", 0);
	skyboxShader->addAttribute("vertexPosition", 3, GL_FLOAT, GL_FALSE, 0, 0);

	// defaultShader
	defaultShader = new Shader("Default", "defaultV.glsl", "defaultF.glsl");

	//TODO: Have a config somewhere load all of these and init these values
	Shader *shapeShader = new Shader("Model", "modelV.glsl", "modelF.glsl");
	//TODO: Shapes
	shapeShader->addUniformLocation("textureSampler", 0);
	//	shapeShader->addUniformLocation("normalSampler", 1);
	//	shapeShader->addUniformLocation("specularSampler", 2);

	// Debug shader
	Shader *debugShader = new Shader("Debug", "debugV.glsl", "debugF.glsl");
	debugShader->addAttribute("vertexPosition", 3, GL_FLOAT, false, sizeof(Point), (void *)offsetof(Point, vertex));
	debugShader->addAttribute("vertexColor", 3, GL_FLOAT, false, sizeof(Point), (void *)offsetof(Point, color));
}