//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef camera_h
#define camera_h

#include "base/types.h"
#include "game/gameObject.h"

/**
 * This class represents a free-form camera that does not view at any specific
 * object. It implements a camera so that the world can be viewed inside of
 * the graphics rendering.
 */
class Camera : public GameObject {
protected:
	/**
	 * The Camera's yaw value.
	 */
	F32 mYaw;
	
	/**
	 * The Camera's pitch value.
	 */
	F32 mPitch;

	/**
	 * A speed factor of which the camera moves based on mouse movement.
	 */
	const F32 mCameraSpeed = 0.3f;

	/**
	 * A speed factor of which the camera moves based on keyboard movement.
	 */
	const F32 mKeyCameraSpeed = 3.f;

public:
	Camera();
	virtual ~Camera() {};

	/**
	 * Updates the Camera.
	 * @param movement The move struct that contains input for this frame.
	 * @param deltaMS The delta between the last and current frame.
	 */
	virtual void updateCamera(const Movement &movement, const F64 &deltaMS);

	/**
	 * Updates the movement of the Camera based upon input and mspf.
	 * @param movement The move struct that contains input for this frame.
	 * @param deltaMS The delta between the last and current frame.
	 */
	virtual void updateMove(const Movement &movement, const F64 &deltaMS);

	/**
	 * Gets the camera position from the Camera's perspective.
	 * @param OUT mat The camera matrix from the Camera's perspective.
	 * @param OUT pos The position of the Camera.
	 */
	virtual void getCameraPosition(glm::mat4x4 &mat, glm::vec3 &pos);

	/**
	 * An update callback that is called every physics tick.
	 * @param deltaMS The delta between physics ticks.
	 * @note Called at a fixed timestep.
	 */
	virtual inline void updateTick(const F64 &deltaMS) {};
};

#endif
