//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

IMPLEMENT_SCRIPTOBJECT(Camera, GameObject);

Camera::Camera() : GameObject() {
	mPitch = 0.0f;
	mYaw = 0.0f;
}

void Camera::updateCamera(const Movement &movement, const F64 &delta) {
	//Keyboard movement
	if (movement.pitchUp)   mPitch -= mKeyCameraSpeed;
	if (movement.pitchDown) mPitch += mKeyCameraSpeed;
	if (movement.yawLeft)   mYaw -= mKeyCameraSpeed;
	if (movement.yawRight)  mYaw += mKeyCameraSpeed;

	//Mouse movement
	// we do not need to use a delta, because SDL resets the delta from the previous call.
	// so it will be same speed regardless of framerate already!
	mPitch += movement.pitch * mCameraSpeed; //*(delta / 0.016f);
	mYaw   += movement.yaw   * mCameraSpeed; //*(delta / 0.016f);
}

void Camera::updateMove(const Movement &movement, const F64 &delta) {
	glm::mat4x4 deltaMat = glm::mat4x4(1);

	//Invert these because we are a free cam
	deltaMat = glm::rotate(deltaMat, -mYaw, glm::vec3(0, 0, 1));
	deltaMat = glm::rotate(deltaMat, -mPitch, glm::vec3(1, 0, 0));

	//Move based on movement keys
	if (movement.forward)  deltaMat = glm::translate(deltaMat, glm::vec3(0, 1, 0));
	if (movement.backward) deltaMat = glm::translate(deltaMat, glm::vec3(0, -1, 0));
	if (movement.left)     deltaMat = glm::translate(deltaMat, glm::vec3(-1, 0, 0));
	if (movement.right)    deltaMat = glm::translate(deltaMat, glm::vec3(1, 0, 0));

	//Move the origin
	mPosition += glm::vec3(deltaMat[3]) * F32(delta / 0.016f);
}

void Camera::getCameraPosition(glm::mat4 &mat, glm::vec3 &pos) {
	//Reset the matrix
	mat = glm::mat4(1.0f);

	//Rotate camera around the origin
	mat = glm::rotate(mat, mPitch, glm::vec3(1.0f, 0.0f, 0.0f));
	mat = glm::rotate(mat, mYaw, glm::vec3(0.0f, 0.0f, 1.0f));

	//Offset the camera by the negative position to bring us into the center.
	// This is not affected by pitch/yaw
	mat = glm::translate(mat, -mPosition);

	pos = mPosition;
}

void Camera::initScript(ScriptEngine *engine) {
	engine->addClass<Camera, GameObject>("Camera");
	sConcreteClassRep.addSimpleField(engine, &Camera::mYaw, "yaw");
	sConcreteClassRep.addSimpleField(engine, &Camera::mPitch, "pitch");
}