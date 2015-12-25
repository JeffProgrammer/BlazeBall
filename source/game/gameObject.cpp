//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "gameObject.h"
#include <glm/gtc/matrix_transform.hpp>

ConcreteClassRep<GameObject> GameObject::sConcreteClassRep("GameObject");

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
	sConcreteClassRep.addSimpleField<glm::vec3>("position", offsetof(GameObject, mPosition));
	sConcreteClassRep.addSimpleField<glm::quat>("rotation", offsetof(GameObject, mRotation));
	sConcreteClassRep.addSimpleField<glm::vec3>("scale",    offsetof(GameObject, mScale));
}