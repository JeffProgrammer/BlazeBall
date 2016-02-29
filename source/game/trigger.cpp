//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "trigger.h"
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

IMPLEMENT_SCRIPTOBJECT(Trigger, GameObject);

Trigger::Trigger() {
	mTrigger = nullptr;
}

Trigger::~Trigger() {
	delete mTrigger;
}

void Trigger::onAddToScene() {
	Parent::onAddToScene();
}

bool Trigger::read(CharStream &stream) {
	if (!Parent::read(stream))
		return false;
	return true;
}

bool Trigger::write(CharStream &stream) const {
	if (!Parent::write(stream))
		return false;
	return true;
}

void Trigger::updateTick(const F64 &dt) {
	Parent::updateTick(dt);
}

void Trigger::onEnterTrigger(GameObject *collider) {

}

void Trigger::onLeaveTrigger(GameObject *collider) {

}

void Trigger::initFields() {

}