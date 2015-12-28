//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "gameObject.h"
#include <glm/gtc/matrix_transform.hpp>

IMPLEMENT_SCRIPTOBJECT(GameObject, ScriptObject);

void GameObject::updateCamera(const Movement &movement, const F64 &delta) {
	//Nothing
}
void GameObject::updateMove(const Movement &movement, const F64 &delta) {
	//Nothing
}
void GameObject::getCameraPosition(glm::mat4x4 &mat, glm::vec3 &pos) {
	mat = glm::mat4(1);
	mat = glm::translate(mat, glm::vec3(-mPosition.x, -mPosition.y, -mPosition.z));
	pos = mPosition;
}

void GameObject::initFields() {
	AddFieldSimple("position", glm::vec3, offsetof(GameObject, mPosition));
	AddFieldSimple("rotation", glm::quat, offsetof(GameObject, mRotation));
	AddFieldSimple("scale",    glm::vec3, offsetof(GameObject, mScale));
}