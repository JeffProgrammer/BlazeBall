//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GAME_PHYSICS_BULLET_BTDEBUGDRAWER_H_
#define _GAME_PHYSICS_BULLET_BTDEBUGDRAWER_H_

#include "btPhysicsEngine.h"
#include "render/shader.h"
#include "render/renderInfo.h"
#include <vector>
#include <glm/glm.hpp>

class btDebugDrawer : public btIDebugDraw {
	std::vector<Point> mLinePoints;
	Shader *mShader;
	GLuint mVertexBuffer;

	int mDebugMode;

public:
	btDebugDrawer();

	virtual void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color);
	virtual void draw3dText(const btVector3 &location, const char *textString);
	virtual void setDebugMode(int debugMode);
	virtual int  getDebugMode() const;
	virtual void drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color);
	virtual void reportErrorWarning(const char *warningString);

	void draw(RenderInfo &info, const PhysicsEngine::DebugDrawType &drawType);
};

#endif // _GAME_PHYSICS_BULLET_BTDEBUGDRAWER_H_
