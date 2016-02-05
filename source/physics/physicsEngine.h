//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef physicsEngine_h
#define physicsEngine_h

#include <stdio.h>
#include <functional>
#include "base/types.h"
#include "render/renderInfo.h"

#define PHYSICS_TICK 0.01666

class PhysicsBody;
class GameInterior;

class PhysicsEngine {
public:
	typedef std::function<void(const F64 &)> StepCallback;
protected:
	bool mRunning;

	StepCallback stepCallback;

	inline virtual void step(const F64 &delta) {
		this->stepCallback(delta);
	}
public:
	PhysicsEngine() : mRunning(false) {}
	virtual ~PhysicsEngine() {}

	virtual void init() = 0;
	virtual void simulate(const F64 &delta) = 0;
	virtual void addBody(PhysicsBody *body) = 0;

	virtual PhysicsBody *createInterior(GameInterior *interior) = 0;
	virtual PhysicsBody *createSphere(const F32 &radius) = 0;

	void setRunning(bool running) { this->mRunning = running; }
	bool getRunning() { return mRunning; }

	void setStepCallback(StepCallback callback) { this->stepCallback = callback; }

	struct RaycastInfo {
		glm::vec3 from;
		glm::vec3 to;
		bool hit;

		glm::vec3 point;
		glm::vec3 normal;		
		void *body;
	};

	virtual void raycast(RaycastInfo &info) = 0;

	enum DebugDrawType {
		Everything,
		Nothing
	};

	virtual void debugDraw(RenderInfo &info, const DebugDrawType &drawType) = 0;

};

#endif
