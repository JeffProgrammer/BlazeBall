//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "renderedObject.h"

IMPLEMENT_SCRIPTOBJECT(RenderedObject, GameObject);

void RenderedObject::calculateModelMatrix(const RenderInfo &info, glm::mat4 &modelMatrix) {
	glm::vec3 pos = getPosition();
	glm::quat rot = getRotation();
	glm::vec3 scale = getScale();

	//Model
	modelMatrix = glm::mat4x4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(pos.x, pos.y, pos.z));
	modelMatrix = glm::rotate(modelMatrix, glm::angle(rot), glm::axis(rot));
	modelMatrix = glm::scale(modelMatrix, scale);
}

void RenderedObject::loadModelMatrix(const RenderInfo &info, Shader *shader) {
	//Load the model matrix
	glm::mat4 modelMatrix(1);
	//Separate function call here because this is virtual (and overridden)
	calculateModelMatrix(info, modelMatrix);

	//Inverse (because some shaders need it)
	glm::mat4 inverseModelMatrix = glm::inverse(modelMatrix);

	//Load it into the shader
	shader->setUniformMatrix("modelMat", GL_FALSE, modelMatrix);
	shader->setUniformMatrix("inverseModelMat", GL_FALSE, inverseModelMatrix);
}

void RenderedObject::initFields() {
	// Nothing at the moment.
}

void RenderedObject::initScript(ScriptEngine *engine) {
	// Nothing.
}