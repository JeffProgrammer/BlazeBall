//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef camera_h
#define camera_h

#include "base/types.h"
#include "game/gameObject.h"

class Camera : public GameObject {
	F32 yaw;
	F32 pitch;

	const F32 cameraSpeed = 0.3f;
	const F32 keyCameraSpeed = 3.f;

public:
	Camera();
	virtual ~Camera() {};

	virtual void updateCamera(const Movement &movement, const F64 &deltaMS);
	virtual void updateMove(const Movement &movement, const F64 &deltaMS);
	virtual void getCameraPosition(glm::mat4x4 &mat, glm::vec3 &pos);
	virtual inline void updateTick(const F64 &deltaMS) {};
};

#endif
