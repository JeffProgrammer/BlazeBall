//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
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
}

void World::tick(const F64 &delta) {
	for (auto object : mObjects) {
		object->updateTick(delta);
	}
}

GameObject* World::findGameObject(const std::string &name) {
	// O(n)
	// TODO: store objects in a hash map or something.
	for (const auto obj : mObjects) {
		if (obj->getName() == name)
			return obj;
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

			// Make sure that the field only is 1 word. If not reject it
			if (temp___containsMoreThanOneWord(fieldName)) {
				IO::printf("Field %s was rejected because a field name must only be one word!\n", fieldName);
				continue;
			}

			if (field->value.IsArray()) {
				// Behaviors are an array of behavior objects.
				// we create them here.
				if (stricmp(fieldName, "behaviors") == 0) {
					for (auto name = field->value.Begin(); name != field->value.End(); ++name) {
						const char *behaviorName = name->GetString();
						auto behavior = BehaviorAbstractClassRep::createFromName(behaviorName);
						if (behavior == nullptr) {
							IO::printf("Could not create behavior named %s.\n", behaviorName);
						} else {
							scriptObject->addBehavior(behavior);
						}
					}
				}
			} else {
				const char *fieldValue = field->value.GetString();

				// we allready got the class field.
				if (stricmp(fieldName, "class") == 0)
					continue;

				//Try and set the field
				if (!scriptObject->setField(fieldName, fieldValue)) {
					IO::printf("Could not set class field %s on an object of type %s.\n", fieldName, klass);
				}
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
