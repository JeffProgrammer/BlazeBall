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
	mat = Mat4::translate(mat, Vec3(-mPosition.x, -mPosition.y, -mPosition.z));
	pos = mPosition;
}

void GameObject::initFields() {
	AddFieldSimple("position", glm::vec3, &GameObject::mPosition);
	AddFieldSimple("rotation", glm::quat, &GameObject::mRotation);
	AddFieldSimple("scale",    glm::vec3, &GameObject::mScale);
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

	setPosition(stream.pop<Vec3>());
	setRotation(stream.pop<Quat>());
	setScale   (stream.pop<Vec3>());

	return true;
}
bool GameObject::write(CharStream &stream) const {
	if (!NetObject::write(stream)) {
		return false;
	}

	stream.push<Vec3>(getPosition());
	stream.push<Quat>(getRotation());
	stream.push<Vec3>(getScale());

	return true;
}
