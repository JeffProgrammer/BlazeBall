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

#include "game/gameObject.h"
#include "behaviors/behavior.h"
#include <glm/gtc/matrix_transform.hpp>

IMPLEMENT_SCRIPTOBJECT(GameObject, ScriptObject);

void GameObject::updateCamera(const Movement &movement, const F64 &delta) {
	//Nothing
}

void GameObject::updateMove(const Movement &movement, const F64 &delta) {
	//Nothing
}

void GameObject::updateTick(const F64 &dt) {
	for (auto behavior : mBehaviors)
		behavior->updateTick(dt);
}

void GameObject::getCameraPosition(glm::mat4 &mat, glm::vec3 &pos) {
	mat = glm::mat4(1.0f);
	mat = glm::translate(mat, glm::vec3(-mPosition.x, -mPosition.y, -mPosition.z));
	pos = mPosition;
}	

void GameObject::initFields() {
	AddField(GameObject::mPosition, "position");
	AddField(GameObject::mRotation, "rotation");
	AddField(GameObject::mScale, "scale");
}

bool GameObject::read(CharStream &stream) {
	if (!NetObject::read(stream)) {
		return false;
	}

	setPosition(stream.pop<glm::vec3>());
	setRotation(stream.pop<glm::quat>());
	setScale   (stream.pop<glm::vec3>());

	return true;
}
bool GameObject::write(CharStream &stream) const {
	if (!NetObject::write(stream)) {
		return false;
	}

	stream.push<glm::vec3>(getPosition());
	stream.push<glm::quat>(getRotation());
	stream.push<glm::vec3>(getScale());

	return true;
}
