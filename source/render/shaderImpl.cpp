//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the project nor the
//      names of its contributors may be used to endorse or promote products
//      derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

#include "render/shader.h"
#include "base/math.h"
#include "render/renderInfo.h"

void Shader::initializeShaders() {
	// Interior shader
	Shader *interiorShader = new Shader("Interior", "interiorV.glsl", "interiorF.glsl");
	interiorShader->addUniformLocation(SAMPLER_TEXTURE_NAME,  0);
	interiorShader->addUniformLocation(SAMPLER_NORMAL_NAME,   1);
	interiorShader->addUniformLocation(SAMPLER_SPECULAR_NAME, 2);
	interiorShader->addUniformLocation(SAMPLER_NOISE_NAME,    3);
	interiorShader->addUniformLocation(SAMPLER_CUBEMAP_NAME,  4);

	interiorShader->addAttribute(ATTRIBUTE_POSITION_NAME,  3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, point)));
	interiorShader->addAttribute(ATTRIBUTE_UV_NAME,        2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));
	interiorShader->addAttribute(ATTRIBUTE_NORMAL_NAME,    3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
	interiorShader->addAttribute(ATTRIBUTE_TANGENT_NAME,   3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tangent)));
	interiorShader->addAttribute(ATTRIBUTE_BITANGENT_NAME, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, bitangent)));

	// Sphere shader
	Shader *sphereShader = new Shader("Sphere", "sphereV.glsl", "sphereF.glsl");
	sphereShader->addUniformLocation(SAMPLER_TEXTURE_NAME,  0);
	sphereShader->addUniformLocation(SAMPLER_NORMAL_NAME,   1);
	sphereShader->addUniformLocation(SAMPLER_SPECULAR_NAME, 2);
	sphereShader->addUniformLocation(SAMPLER_CUBEMAP_NAME,  3);
	sphereShader->addAttribute(ATTRIBUTE_POSITION_NAME,  3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, point)));
	sphereShader->addAttribute(ATTRIBUTE_UV_NAME,        2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));
	sphereShader->addAttribute(ATTRIBUTE_NORMAL_NAME,    3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
	sphereShader->addAttribute(ATTRIBUTE_TANGENT_NAME,   3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tangent)));
	sphereShader->addAttribute(ATTRIBUTE_BITANGENT_NAME, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, bitangent)));

	// Skybox shader
	Shader *skyboxShader = new Shader("Skybox", "skyboxV.glsl", "skyboxF.glsl");
	skyboxShader->addUniformLocation(SAMPLER_CUBEMAP_NAME, 0);
	skyboxShader->addAttribute(ATTRIBUTE_POSITION_NAME, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// defaultShader
	defaultShader = new Shader("Default", "defaultV.glsl", "defaultF.glsl");

	//TODO: Have a config somewhere load all of these and init these values
	Shader *shapeShader = new Shader("Model", "modelV.glsl", "modelF.glsl");
	shapeShader->addAttribute(ATTRIBUTE_POSITION_NAME, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, point)));
	shapeShader->addAttribute(ATTRIBUTE_UV_NAME, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));
	shapeShader->addUniformLocation(SAMPLER_TEXTURE_NAME, 0);
	//	shapeShader->addUniformLocation(SAMPLER_NORMAL_NAME, 1);
	//	shapeShader->addUniformLocation(SAMPLER_SPECULAR_NAME, 2);

	// Debug shader
	Shader *debugShader = new Shader("Debug", "debugV.glsl", "debugF.glsl");
	debugShader->addAttribute(ATTRIBUTE_POSITION_NAME, 3, GL_FLOAT, false, sizeof(Point), (void *)offsetof(Point, vertex));
	debugShader->addAttribute(ATTRIBUTE_COLOR_NAME,    3, GL_FLOAT, false, sizeof(Point), (void *)offsetof(Point, color));

	Shader *postFXShader = new Shader("PostFX", "postfxV.glsl", "postfxF.glsl");
	postFXShader->addUniformLocation(SAMPLER_TEXTURE_NAME, 0);
	postFXShader->addUniformLocation(SAMPLER_DEPTH_NAME,   1);
	postFXShader->addAttribute(ATTRIBUTE_POSITION_NAME, 3, GL_FLOAT, GL_FALSE);

	// Trigger shader
	Shader *triggerShader = new Shader("Trigger", "triggerV.glsl", "triggerF.glsl");
	triggerShader->addAttribute(ATTRIBUTE_POSITION_NAME, 3, GL_FLOAT, GL_FALSE, 0, 0);

}
