//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
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
void GameObject::getCameraPosition(glm::mat4 &mat, glm::vec3 &pos) {
	mat = glm::mat4(1.0f);
	mat = glm::translate(mat, glm::vec3(-mPosition.x, -mPosition.y, -mPosition.z));
	pos = mPosition;
}	

void GameObject::initScript(ScriptEngine *engine) {
	engine->addClass<GameObject, ScriptObject>("GameObject");
	engine->addMethod(&GameObject::getPosition, "getPosition");
	engine->addMethod(&GameObject::setPosition, "setPosition");
	engine->addMethod(&GameObject::getRotation, "getRotation");
	engine->addMethod(&GameObject::setRotation, "setRotation");
	engine->addMethod(&GameObject::getScale, "getScale");
	engine->addMethod(&GameObject::setScale, "setScale");

	sConcreteClassRep.addSimpleField(engine, &GameObject::mPosition, "position");
	sConcreteClassRep.addSimpleField(engine, &GameObject::mRotation, "rotation");
	sConcreteClassRep.addSimpleField(engine, &GameObject::mScale, "scale");
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
