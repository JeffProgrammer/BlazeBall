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

#include "graphics/shader.h"

struct RenderInfo {
	static glm::mat4 inverseRotMat;

	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::vec3 cameraPosition;

	glm::vec4 lightColor;
	glm::vec4 ambientColor;
	glm::vec3 sunPosition;
	U32 specularExponent;

	inline void loadShader(Shader *shader) const {
		if (shader->getUniformLocation(PROJECTION_MATRIX_UNIFORM_NAME) != -1)
			glUniformMatrix4fv(shader->getUniformLocation(PROJECTION_MATRIX_UNIFORM_NAME),   1, GL_FALSE, &projectionMatrix[0][0]);
		if (shader->getUniformLocation(VIEW_MATRIX_UNIFORM_NAME) != -1)
			glUniformMatrix4fv(shader->getUniformLocation(VIEW_MATRIX_UNIFORM_NAME),         1, GL_FALSE, &viewMatrix[0][0]);
		if (shader->getUniformLocation(INVERSE_VIEW_MATRIX_UNIFORM_NAME) != -1)
			glUniformMatrix4fv(shader->getUniformLocation(INVERSE_VIEW_MATRIX_UNIFORM_NAME), 1, GL_FALSE, &inverseRotMat[0][0]);
		if (shader->getUniformLocation(CAMERA_POSITION_UNIFORM_NAME) != -1)
			glUniform3fv      (shader->getUniformLocation(CAMERA_POSITION_UNIFORM_NAME),     1,           &cameraPosition[0]);
		if (shader->getUniformLocation(LIGHT_COLOR_UNIFORM_NAME) != -1)
			glUniform4fv      (shader->getUniformLocation(LIGHT_COLOR_UNIFORM_NAME),         1,           &lightColor.r);
		if (shader->getUniformLocation(AMBIENT_LIGHT_COLOR_UNIFORM_NAME) != -1)
			glUniform4fv      (shader->getUniformLocation(AMBIENT_LIGHT_COLOR_UNIFORM_NAME), 1,           &ambientColor.r);
		if (shader->getUniformLocation(SUN_POSITION_UNIFORM_NAME) != -1)
			glUniform3fv      (shader->getUniformLocation(SUN_POSITION_UNIFORM_NAME),        1,           &sunPosition.x);
		if (shader->getUniformLocation(SPECULAR_EXPONENT_UNIFORM_NAME) != -1)
			glUniform1f       (shader->getUniformLocation(SPECULAR_EXPONENT_UNIFORM_NAME),                static_cast<F32>(specularExponent));
	}
};

#endif 