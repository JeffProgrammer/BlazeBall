//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
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
	Mat4 deltaMat = Mat4(1.0f);

	//Invert these because we are a free cam
	deltaMat = Mat4::rotate(deltaMat, -mYaw, Vec3(0, 0, 1));
	deltaMat = Mat4::rotate(deltaMat, -mPitch, Vec3(1, 0, 0));

	//Move based on movement keys
	if (movement.forward)  deltaMat = Mat4::translate(deltaMat, Vec3(0, 1, 0));
	if (movement.backward) deltaMat = Mat4::translate(deltaMat, Vec3(0, -1, 0));
	if (movement.left)     deltaMat = Mat4::translate(deltaMat, Vec3(-1, 0, 0));
	if (movement.right)    deltaMat = Mat4::translate(deltaMat, Vec3(1, 0, 0));

	//Move the origin
	mPosition += Vec3(deltaMat[3].x, deltaMat[3].y, deltaMat[3].z) * F32(delta / 0.016f);
}

void Camera::getCameraPosition(Mat4 &mat, Vec3 &pos) {
	//Reset the matrix
	mat = Mat4(1.0f);

	//Rotate camera around the origin
	mat = Mat4::rotate(mat, mPitch, Vec3(1.0f, 0.0f, 0.0f));
	mat = Mat4::rotate(mat, mYaw, Vec3(0.0f, 0.0f, 1.0f));

	//Offset the camera by the negative position to bring us into the center.
	// This is not affected by pitch/yaw
	mat = Mat4::translate(mat, -mPosition);

	pos = mPosition;
}

void Camera::initScript(ScriptEngine *engine) {
	engine->addClass<Camera, GameObject>("Camera");
	engine->addField(&Camera::mYaw, "yaw");
	engine->addField(&Camera::mPitch, "pitch");
}