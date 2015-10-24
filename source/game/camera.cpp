//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() {
	pitch = 0.0f;
	yaw = 0.0f;
}

void Camera::updateCamera(const Movement &movement, const F64 &deltaMS) {
	//Keyboard movement
	if (movement.pitchUp)   pitch -= keyCameraSpeed;
	if (movement.pitchDown) pitch += keyCameraSpeed;
	if (movement.yawLeft)   yaw -= keyCameraSpeed;
	if (movement.yawRight)  yaw += keyCameraSpeed;

	//Mouse movement
	// we do not need to use a delta, because SDL resets the delta from the previous call.
	// so it will be same speed regardless of framerate already!
	pitch += movement.pitch * cameraSpeed; //*(deltaMS / 16.f);
	yaw   += movement.yaw   * cameraSpeed; //*(deltaMS / 16.f);
}

void Camera::updateMove(const Movement &movement, const F64 &deltaMS) {
	glm::mat4x4 delta = glm::mat4x4(1);

	//Invert these because we are a free cam
	delta = glm::rotate(delta, -yaw, glm::vec3(0, 0, 1));
	delta = glm::rotate(delta, -pitch, glm::vec3(1, 0, 0));

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
	mat = glm::rotate(mat, pitch, glm::vec3(1, 0, 0));
	mat = glm::rotate(mat, yaw, glm::vec3(0, 0, 1));

	//Offset the camera by the negative position to bring us into the center.
	// This is not affected by pitch/yaw
	mat = glm::translate(mat, -mOrigin);

	pos = mOrigin;
}
