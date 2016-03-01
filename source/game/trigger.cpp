//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "game/trigger.h"
#include "game/sphere.h"
#include "render/triggerData.h"
#include "behaviors/triggerBehavior.h"
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

IMPLEMENT_SCRIPTOBJECT(Trigger, GameObject);

Trigger::Trigger() {
	mTrigger = nullptr;
	mGenerated = false;
}

Trigger::~Trigger() {
	delete mTrigger;
	delete mMaterial;
	
	glDeleteBuffers(1, &mBuffer);
}

void Trigger::onAddToScene() {
	Parent::onAddToScene();

	mTrigger = new PhysicsTrigger(this);
	mTrigger->setWorld(mWorld);
	mWorld->getPhysicsEngine()->addBody(mTrigger);
}

void Trigger::generateBuffer() {
	glGenBuffers(1, &mBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * sizeof(TriggerData::cubeVertices), TriggerData::cubeVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &mLineBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mLineBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * sizeof(TriggerData::lineVertices), TriggerData::lineVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	Shader *shader = Shader::getShaderByName("Trigger");
	mMaterial = new Material("", shader);

	mGenerated = true;
}

void Trigger::draw(Material *material, RenderInfo &info, void *userInfo) {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LESS);

	Shader *shader = mMaterial->getShader();

	mMaterial->activate();
	info.loadShader(shader);

	loadModelMatrix(info, shader);

	glBindBuffer(GL_ARRAY_BUFFER, mBuffer);
	shader->enableAttributes();
	shader->setUniform("inLines", 0);

	//Simple draw
	glDrawArrays(GL_TRIANGLES, 0, 36); //Hardcoded count from # of verts above
	epoxy_glFrontFace(GL_CW);
	glDrawArrays(GL_TRIANGLES, 0, 36); //Hardcoded count from # of verts above
	epoxy_glFrontFace(GL_CCW);

	glBindBuffer(GL_ARRAY_BUFFER, mLineBuffer);
	shader->enableAttributes();
	shader->setUniform("inLines", 1);
	glDrawArrays(GL_LINES, 0, sizeof(TriggerData::lineVertices) / 12);

	shader->disableAttributes();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	mMaterial->deactivate();

	glDisable(GL_BLEND);
}

void Trigger::render(RenderInfo &info) {
	if (!mGenerated) {
		generateBuffer();
	}

	info.addRenderMethod(mMaterial, RenderInfo::RenderMethod::from_method<Trigger, &Trigger::draw>(this), nullptr, RenderInfo::RenderOrderPosition::Debug);
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

	// Notify all behaviors that we have entered a trigger
	for (Behavior *b : mBehaviors) {
		// TODO: THIS IS SO SLOW.
		// THIS NEEDS TO BE CHANGED OR ELSE WE CAN EXPERIENCE A PERFORMANCE HIT!!!!!
		TriggerBehavior *tb = dynamic_cast<TriggerBehavior*>(b);
		if (tb != nullptr) {
			tb->onEnterTrigger();
		}
	}
}

void Trigger::onLeaveTrigger(Sphere *collider) {
	IO::printf("%p has left the trigger %p\n", collider, this);

	// Notify all behaviors that we have entered a trigger
	for (Behavior *b : mBehaviors) {
		// TODO: THIS IS SO SLOW.
		// THIS NEEDS TO BE CHANGED OR ELSE WE CAN EXPERIENCE A PERFORMANCE HIT!!!!!
		TriggerBehavior *tb = dynamic_cast<TriggerBehavior*>(b);
		if (tb != nullptr) {
			tb->onLeaveTrigger();
		}
	}
}

void Trigger::initFields() {

}