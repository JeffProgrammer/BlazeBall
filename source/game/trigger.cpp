//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "game/trigger.h"
#include "game/sphere.h"
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

	mTrigger = new PhysicsTrigger(this);
	mTrigger->setWorld(mWorld);
	mWorld->getPhysicsEngine()->addBody(mTrigger);
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

	std::vector<Sphere*> removeList;

	const BoxF &triggerBox = mTrigger->getWorldBox();

	for (Sphere *obj : mObjects) {
		// If the object is not inside of the trigger,
		// issue a onLeaveTrigger() event and
		// remove it from the list.
		if (!triggerBox.intersectsBox(obj->getWorldBox())) {
			removeList.push_back(obj);
			onLeaveTrigger(obj);
		}
	}

	// remove them
	for (Sphere *remove : removeList)
		mObjects.erase(std::find(mObjects.begin(), mObjects.end(), remove));
}

void Trigger::onEnterTrigger(Sphere *collider) {
	// If we already have this object in the list, don't issue another event.
	if (std::find(mObjects.begin(), mObjects.end(), collider) != mObjects.end())
		return;

	IO::printf("%p has entered the trigger %p\n", collider, this);
	mObjects.push_back(collider);
}

void Trigger::onLeaveTrigger(Sphere *collider) {
	IO::printf("%p has left the trigger %p\n", collider, this);
}

void Trigger::initFields() {

}