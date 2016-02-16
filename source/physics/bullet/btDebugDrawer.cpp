//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "btDebugDrawer.h"

btDebugDrawer::btDebugDrawer() : mShader(NULL) {

}

void btDebugDrawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) {
	mLinePoints.push_back(Point(btConvert(from), btConvert(color)));
	mLinePoints.push_back(Point(btConvert(to), btConvert(color)));
}
void btDebugDrawer::draw3dText(const btVector3 &location, const char *textString) {
	//Uh
}
void btDebugDrawer::setDebugMode(int debugMode) {
	mDebugMode = debugMode;
}
int btDebugDrawer::getDebugMode() const {
	return 0xFFFF;
}
void btDebugDrawer::drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color) {
	mLinePoints.push_back(Point(btConvert(PointOnB), btConvert(color)));
	mLinePoints.push_back(Point(btConvert(PointOnB) + btConvert(normalOnB), btConvert(color)));
}
void btDebugDrawer::reportErrorWarning(const char *warningString) {

}

void btDebugDrawer::draw(RenderInfo &info, const PhysicsEngine::DebugDrawType &drawType) {
	if (drawType == PhysicsEngine::DebugDrawType::Nothing) {
		//Dump data and return
		mLinePoints.clear();
		return;
	}

	if (mShader == NULL) {
		mShader = Shader::getShaderByName("Debug");
		glGenBuffers(1, &mVertexBuffer);
	}

	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Point) * mLinePoints.size(), &mLinePoints[0], GL_STREAM_DRAW);
	mShader->activate();

	mShader->enableAttributes();
	mShader->setUniformMatrix(UNIFORM_PROJECTION_MATRIX_NAME, false, info.projectionMatrix);
	mShader->setUniformMatrix(UNIFORM_VIEW_MATRIX_NAME, false, info.viewMatrix);

	glDrawArrays(GL_LINES, 0, mLinePoints.size());

	mShader->disableAttributes();
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	mShader->deactivate();
	mLinePoints.clear();
}
