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

#include "gameObject.h"
#include <glm/gtc/matrix_transform.hpp>

IMPLEMENT_OBJECT(GameObject);

void GameObject::updateCamera(const Movement &movement, const F64 &deltaMS) {
	//Nothing
}
void GameObject::updateMove(const Movement &movement, const F64 &deltaMS) {
	//Nothing
}
void GameObject::getCameraPosition(glm::mat4x4 &mat) {
	mat = glm::mat4(1);
	mat = glm::translate(mat, glm::vec3(-mOrigin.x, -mOrigin.y, -mOrigin.z));
}

void GameObject::loadMatrix(const glm::mat4 &projectionMatrix, const glm::mat4 &viewMatrix, glm::mat4 &modelMatrix) {
	glm::vec3 pos = getPosition();
	glm::quat rot = getRotation();
	glm::vec3 scale = getScale();

	//Model
	modelMatrix = glm::mat4x4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(pos.x, pos.y, pos.z));
	modelMatrix = glm::rotate(modelMatrix, glm::angle(rot), glm::axis(rot));
	modelMatrix = glm::scale(modelMatrix, scale);
}

void GameObject::render() {
	
}

void GameObject::updateTick(const F64 &deltaMS) {
	
}

OBJECT_METHOD(GameObject, getPosition) {
	glm::vec3 pos = object->getPosition();
	EXTERN_OBJECT(glm::vec3, position) = &pos;
	args.GetReturnValue().Set(position);
}

OBJECT_METHOD(GameObject, getRotation) {
	glm::quat rot = object->getRotation();
	EXTERN_OBJECT(glm::quat, rotation) = &rot;
	args.GetReturnValue().Set(rotation);
}

OBJECT_METHOD(GameObject, getScale) {
	glm::vec3 scl = object->getScale();
	EXTERN_OBJECT(glm::vec3, scale) = &scl;
	args.GetReturnValue().Set(scale);
}

OBJECT_METHOD(GameObject, setPosition) {
	glm::vec3 *position = UNWRAP_EXTERN(glm::vec3, args[0]);
	object->setPosition(*position);
}

OBJECT_METHOD(GameObject, setRotation) {
	glm::quat *rotation = UNWRAP_EXTERN(glm::quat, args[0]);
	object->setRotation(*rotation);
}

OBJECT_METHOD(GameObject, setScale) {
	glm::vec3 *scale = UNWRAP_EXTERN(glm::vec3, args[0]);
	object->setScale(*scale);
}
