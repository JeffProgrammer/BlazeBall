//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "renderedObject.h"

IMPLEMENT_SCRIPTOBJECT(RenderedObject, GameObject);

void RenderedObject::calculateModelMatrix(const RenderInfo &info, Mat4 &modelMatrix) {
	Vec3 pos = getPosition();
	Quat rot = getRotation();
	Vec3 scale = getScale();

	//Model
	modelMatrix = Mat4(1.0f);
	modelMatrix = Mat4::translate(modelMatrix, Vec3(pos.x, pos.y, pos.z));
	modelMatrix = Mat4::rotate(modelMatrix, Quat::angle(rot), Quat::axis(rot));
	modelMatrix = Mat4::scale(modelMatrix, scale);
}

void RenderedObject::loadModelMatrix(const RenderInfo &info, Shader *shader) {
	//Load the model matrix
	Mat4 modelMatrix(1.0f);
	//Separate function call here because this is virtual (and overridden)
	calculateModelMatrix(info, modelMatrix);

	//Inverse (because some shaders need it)
	const Mat4 &inverseModelMatrix = Mat4::inverse(modelMatrix);

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