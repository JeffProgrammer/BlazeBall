//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() {
	mPitch = 0.0f;
	mYaw = 0.0f;
}

void Camera::updateCamera(const Movement &movement, const F64 &deltaMS) {
	//Keyboard movement
	if (movement.pitchUp)   mPitch -= mKeyCameraSpeed;
	if (movement.pitchDown) mPitch += mKeyCameraSpeed;
	if (movement.yawLeft)   mYaw -= mKeyCameraSpeed;
	if (movement.yawRight)  mYaw += mKeyCameraSpeed;

	//Mouse movement
	// we do not need to use a delta, because SDL resets the delta from the previous call.
	// so it will be same speed regardless of framerate already!
	mPitch += movement.pitch * mCameraSpeed; //*(deltaMS / 16.f);
	mYaw   += movement.yaw   * mCameraSpeed; //*(deltaMS / 16.f);
}

void Camera::updateMove(const Movement &movement, const F64 &deltaMS) {
	glm::mat4x4 delta = glm::mat4x4(1);

	//Invert these because we are a free cam
	delta = glm::rotate(delta, -mYaw, glm::vec3(0, 0, 1));
	delta = glm::rotate(delta, -mPitch, glm::vec3(1, 0, 0));

	//Move based on movement keys
	if (movement.forward)  delta = glm::translate(delta, glm::vec3(0, 1, 0));
	if (movement.backward) delta = glm::translate(delta, glm::vec3(0, -1, 0));
	if (movement.left)     delta = glm::translate(delta, glm::vec3(-1, 0, 0));
	if (movement.right)    delta = glm::translate(delta, glm::vec3(1, 0, 0));

	//Move the origin
	mOrigin += glm::vec3(delta[3]) * F32(deltaMS / 16.f);
}

void Camera::getCameraPosition(glm::mat4x4 &mat, glm::vec3 &pos) {
	//Reset the matrix
	mat = glm::mat4x4(1);

	//Rotate camera around the origin
	mat = glm::rotate(mat, mPitch, glm::vec3(1, 0, 0));
	mat = glm::rotate(mat, mYaw, glm::vec3(0, 0, 1));

	//Offset the camera by the negative position to bring us into the center.
	// This is not affected by pitch/yaw
	mat = glm::translate(mat, -mOrigin);

	pos = mOrigin;
}
