//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of the project nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
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


#ifndef renderInfo_h
#define renderInfo_h

#include <glm/glm.hpp>

#ifdef _WIN32
#include <GL/glew.h>
#else
#include <OpenGL/gl.h>
#endif

#include "render/shader.h"

//todo: vector also per-shader
#define PROJECTION_MATRIX_UNIFORM_NAME "projectionMat"
#define VIEW_MATRIX_UNIFORM_NAME "viewMat"
#define INVERSE_VIEW_MATRIX_UNIFORM_NAME "inverseViewMat"
#define CAMERA_POSITION_UNIFORM_NAME "cameraPos"

#define LIGHT_COLOR_UNIFORM_NAME "lightColor"
#define AMBIENT_LIGHT_COLOR_UNIFORM_NAME "ambientColor"
#define SUN_POSITION_UNIFORM_NAME "sunPosition"
#define SPECULAR_EXPONENT_UNIFORM_NAME "specularExponent"

struct RenderInfo {
	static glm::mat4 inverseRotMat;

	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::vec3 cameraPosition;

	glm::vec4 lightColor;
	glm::vec4 ambientColor;
	glm::vec3 sunPosition;
	U32 specularExponent;

	bool isReflectionPass;

	inline void loadShader(Shader *shader) const {
		if (shader == nullptr)
			shader = Shader::defaultShader;
		
		shader->setUniformMatrix(PROJECTION_MATRIX_UNIFORM_NAME,   GL_FALSE, projectionMatrix);
		shader->setUniformMatrix(VIEW_MATRIX_UNIFORM_NAME,         GL_FALSE, viewMatrix);
		shader->setUniformMatrix(INVERSE_VIEW_MATRIX_UNIFORM_NAME, GL_FALSE, inverseRotMat);
		shader->setUniformVector(CAMERA_POSITION_UNIFORM_NAME,     cameraPosition);
		shader->setUniformVector(LIGHT_COLOR_UNIFORM_NAME,         lightColor);
		shader->setUniformVector(AMBIENT_LIGHT_COLOR_UNIFORM_NAME, ambientColor);
		shader->setUniformVector(SUN_POSITION_UNIFORM_NAME,        sunPosition);
		shader->setUniform      (SPECULAR_EXPONENT_UNIFORM_NAME,   static_cast<F32>(specularExponent));
	}
};

#endif 
