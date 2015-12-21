//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "game/scriptObject.h"
#include "scriptEngine/abstractClassRep.h"
#include "scriptEngine/concreteClassRep.h"

ConcreteClassRep<ScriptObject> ScriptObject::sConcreteClassRep("ScriptObject");

ScriptObject::ScriptObject() {
	mClassRep = nullptr;
}

ScriptObject::~ScriptObject() {

}

void ScriptObject::addDynamicField(const std::string &name, const std::string &value) {
	if (value == "") {
		printf("Adding dynamic field %s to object %p failed. You cannot add a blank field.\n", name.c_str(), this);
		return;
	}

	// we store as lower case.
	// TODO: check if this creates a memory leak when dynamicFieldList is destroyed
	// TODO: create a stringtable to manage memory.
	const char *k = strlwr(strdup(name.c_str()));

	// if we already have this field
	// just override it and give a warning.
	if (containsField(name))
		printf("object %p already contains the field %s. Replacing value %s with %s\n.", this, name.c_str(), mDynamicFieldList[k], value.c_str());

	mDynamicFieldList[k] = value.c_str();
}

bool ScriptObject::containsField(const std::string &key) {
	const char* k = key.c_str();
	for (auto it : mDynamicFieldList) {
		if (stricmp(k, it.first) == 0)
			return true;
	}
	return false;
}

void ScriptObject::initFields() {

}