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

void Camera::initFields() {
	AddField(Camera::mYaw, "yaw");
	AddField(Camera::mPitch, "pitch");
}