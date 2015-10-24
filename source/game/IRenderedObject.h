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


#ifndef IRenderedObject_h
#define IRenderedObject_h

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "gameObject.h"
#include "graphics/shader.h"
#include "render/renderInfo.h"

class IRenderedObject : public GameObject {
public:
	virtual void render(const RenderInfo &info) = 0;
	virtual bool isRenderable() { return true; }

	inline virtual void loadMatrix(const glm::mat4 &projectionMatrix, const glm::mat4 &viewMatrix, glm::mat4 &modelMatrix) {
		glm::vec3 pos = getPosition();
		glm::quat rot = getRotation();
		glm::vec3 scale = getScale();

		//Model
		modelMatrix = glm::mat4x4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(pos.x, pos.y, pos.z));
		modelMatrix = glm::rotate(modelMatrix, glm::angle(rot), glm::axis(rot));
		modelMatrix = glm::scale(modelMatrix, scale);
	}
	inline void loadModelMatrix(const RenderInfo &info, Shader *shader) {
		glm::mat4 modelMatrix(1);
		loadMatrix(info.projectionMatrix, info.viewMatrix, modelMatrix);
		glUniformMatrix4fv(shader->getUniformLocation("modelMat"), 1, GL_FALSE, &modelMatrix[0][0]);
		glm::mat4 inverseModelMatrix = glm::inverse(modelMatrix);
		glUniformMatrix4fv(shader->getUniformLocation("inverseModelMat"), 1, GL_FALSE, &inverseModelMatrix[0][0]);
	}
};

#endif
