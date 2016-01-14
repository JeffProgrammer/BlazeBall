//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "gameObject.h"

IMPLEMENT_SCRIPTOBJECT(GameObject, ScriptObject);

void GameObject::updateCamera(const Movement &movement, const F64 &delta) {
	//Nothing
}
void GameObject::updateMove(const Movement &movement, const F64 &delta) {
	//Nothing
}
void GameObject::getCameraPosition(Mat4 &mat, Vec3 &pos) {
	mat = Mat4(1.0f);
	mat = mat.translate(Vec3(-mPosition.x, -mPosition.y, -mPosition.z));
	pos = mPosition;
}

void GameObject::initFields() {
	AddFieldSimple("position", glm::vec3, offsetof(GameObject, mPosition));
	AddFieldSimple("rotation", glm::quat, offsetof(GameObject, mRotation));
	AddFieldSimple("scale",    glm::vec3, offsetof(GameObject, mScale));
}

void GameObject::initScript(ScriptEngine *engine) {
	engine->addClass<GameObject, ScriptObject>("GameObject");
	engine->addMethod(&GameObject::getPosition, "getPosition");
	engine->addMethod(&GameObject::setPosition, "setPosition");
	engine->addMethod(&GameObject::getRotation, "getRotation");
	engine->addMethod(&GameObject::setRotation, "setRotation");
	engine->addMethod(&GameObject::getScale, "getScale");
	engine->addMethod(&GameObject::setScale, "setScale");
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
