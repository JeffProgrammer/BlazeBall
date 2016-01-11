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
	const F32 mCameraSpeed = 0.005f;

	/**
	 * A speed factor of which the camera moves based on keyboard movement.
	 */
	const F32 mKeyCameraSpeed = 0.05f;

	/**
	 * The class rep representing the Camera class.
	 */
	DECLARE_SCRIPTOBJECT(Camera);

public:
	Camera(World *world);
	virtual ~Camera() {};

	/**
	 * Updates the Camera.
	 * @param movement The move struct that contains input for this frame.
	 * @param delta The delta, in seconds, between the last and current frame.
	 */
	virtual void updateCamera(const Movement &movement, const F64 &deltaMS) override;

	/**
	 * Updates the movement of the Camera based upon input and mspf.
	 * @param movement The move struct that contains input for this frame.
	 * @param delta The delta, in seconds, between the last and current frame.
	 */
	virtual void updateMove(const Movement &movement, const F64 &deltaMS) override;

	/**
	 * Gets the camera position from the Camera's perspective.
	 * @param OUT mat The camera matrix from the Camera's perspective.
	 * @param OUT pos The position of the Camera.
	 */
	virtual void getCameraPosition(glm::mat4x4 &mat, glm::vec3 &pos) override;

	/**
	 * An update callback that is called every physics tick.
	 * @param delta The delta, in seconds, between physics ticks.
	 * @note Called at a fixed timestep.
	 */
	virtual void updateTick(const F64 &delta) override {};

	/**
	 * Initializes the fields specific to the GameObject class.
	 */
	static void initFields();

	/**
	 * Initializes the scripting API for the respective sript engine.
	 * @param engine The script engine to initialize to.
	 */
	static void initScript(ScriptEngine *engine);
};

#endif
