//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of the project nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
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
	pitch += movement.pitch * cameraSpeed * (deltaMS / 16.f);
	yaw   += movement.yaw   * cameraSpeed * (deltaMS / 16.f);
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

void Camera::getCameraPosition(glm::mat4x4 &mat) {
	//Reset the matrix
	mat = glm::mat4x4(1);

	//Rotate camera around the origin
	mat = glm::rotate(mat, pitch, glm::vec3(1, 0, 0));
	mat = glm::rotate(mat, yaw, glm::vec3(0, 0, 1));

	//Offset the camera by the negative position to bring us into the center.
	// This is not affected by pitch/yaw
	mat = glm::translate(mat, -mOrigin);
}
