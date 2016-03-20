//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the project nor the
//      names of its contributors may be used to endorse or promote products
//      derived from this software without specific prior written permission.
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

#ifndef _GAME_CAMERA_H_
#define _GAME_CAMERA_H_

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
	Camera();
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
	virtual void getCameraPosition(glm::mat4 &mat, glm::vec3 &pos) override;

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
};

#endif // _GAME_CAMERA_H_