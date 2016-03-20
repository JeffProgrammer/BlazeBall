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

#include "world.h"

#include <rapidjson/document.h>
#include "game/gameObject.h"
#include "behaviors/behaviorConcreteClassRep.h"
#include "behaviors/behavior.h"

#ifdef __APPLE__
#define stricmp strcasecmp
#else
//#define stricmp stricmp
#endif

IMPLEMENT_SCRIPTOBJECT(World, ScriptObject);

World::World(PhysicsEngine *physics) : mPhysicsEngine(physics) {
	mPhysicsEngine->init();
	mPhysicsEngine->setStepCallback([this](F64 delta){
		this->tick(delta);
	});

	mRunning = true;
	mShouldSleep = false;
}

World::~World() {
	for (auto object : mObjects) {
		delete object;
	}
	delete mPhysicsEngine;
}

void World::loop(const F64 &delta) {
	mPhysicsEngine->simulate(delta);

	// Call update on all objects for their behaviors
	for (auto object : mObjects)
		object->update(delta);
}

void World::tick(const F64 &delta) {
	for (auto object : mObjects) {
		// TODO: create custom RTII system because this will be slow as fuck
		// TODO: should be PhysicsObject, not GameObject once we implement that.
		auto go = dynamic_cast<GameObject*>(object);
		if (go != nullptr)
			go->updateTick(delta);
	}
}

GameObject* World::findGameObject(const std::string &name) {
	// O(n)
	// TODO: store objects in a hash map or something.
	for (const auto obj : mObjects) {
		if (obj->getName() == name)
			return dynamic_cast<GameObject*>(obj);
	}
	return nullptr;
}


/* Temporary function until str functions are put in place */
inline bool temp___containsMoreThanOneWord(const std::string &str) {
	size_t len = str.length();
	for (size_t i = 0; i < len; i++) {
		if (str[i] == ' ')
			return true;
	}
	return false;
}

bool World::loadLevel(const std::string &file) {
	if (!IO::isfile(file)) {
		IO::printf("Unable to load level file: %s\n", file.c_str());
		return false;
	}

	// read contents of the json file
	U32 fileLength;
	const char *contents = reinterpret_cast<const char*>(IO::readFile(file, fileLength));

	rapidjson::Document document;
	document.Parse(contents);

	for (auto obj = document.Begin(); obj != document.End(); ++obj) {
		// make sure this is an object. We can only have an array of objects.
		if (!obj->IsObject()) {
			IO::printf("The level file %s can only have an arra of objects within the JSON structure.\n", file.c_str());
			continue;
		}

		// get class name
		if (!obj->HasMember("class")) {
			IO::printf("In %s, an object could not created as a class field was not specified.\n", file.c_str());
			continue;
		}
		const char *klass = (*obj)["class"].GetString();

		// create object here.
		ScriptObject *scriptObject = AbstractClassRep::createFromName(klass, this);

		// loop through each field
		for (auto field = obj->MemberBegin(); field != obj->MemberEnd(); ++field) {
			if (!field->name.IsString()) {
				IO::printf("In %s, all object field names must declared in levels must be strings.\n", file.c_str());
				continue;
			}
			const char *fieldName = field->name.GetString();
			const char *fieldValue = "";

			// Make sure that the field only is 1 word. If not reject it
			if (temp___containsMoreThanOneWord(fieldName)) {
				IO::printf("Field %s was rejected because a field name must only be one word!\n", fieldName);
				continue;
			}

			// we allready got the class field.
			if (stricmp(fieldName, "class") == 0)
				continue;

			if (field->value.IsArray()) {
				// Behaviors are an array of behavior objects.
				// we create them here.
				if (stricmp(fieldName, "behaviors") == 0) {
					std::string behaviorList = "";
					for (auto name = field->value.Begin(); name != field->value.End(); ++name) {
						std::string behaviorName = name->GetString();
						auto behavior = BehaviorAbstractClassRep::createFromName(behaviorName);
						if (behavior == nullptr) {
							IO::printf("Could not create behavior named %s.\n", behaviorName.c_str());
							continue;
						} else {
							scriptObject->addBehavior(behavior);
						}

						// append to behavior list
						if (behaviorList == "")
							behaviorList = behaviorName;
						else
							behaviorList += " " + behaviorName;
					}
					
					// set field behaviors to a list of them.
					fieldValue = behaviorList.c_str();
					scriptObject->setBehaviorString(fieldValue);
				}
			} else {
				fieldValue = field->value.GetString();
			}

			//Try and set the field
			if (!scriptObject->setField(fieldName, fieldValue)) {
				IO::printf("Could not set class field %s on an object of type %s.\n", fieldName, klass);
			}
		}

		// This is a shitty way of doing this, but this is a level loader.
		// If the object is a game object, add it to the scene.
		GameObject *gameObject = dynamic_cast<GameObject *>(scriptObject);
		if (gameObject != nullptr) {
			// add it to the scene.
			addObject(gameObject);
		}

		// for each behavior, call the start method on them.
		auto behaviors = scriptObject->getBehaviors();
		for (Behavior *behavior : behaviors) {
			behavior->start(scriptObject);
		}
	}

	delete[] contents;
	return true;
}

void World::addObject(GameObject *object) {
	mObjects.push_back(object);
	object->onAddToScene();
}

void World::initFields() {
	AddField(World::mSimulationSpeed, "simulationSpeed");
	AddField(World::mRunning, "running");
}
