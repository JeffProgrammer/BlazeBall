//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "game/scriptObject.h"
#include "scriptEngine/abstractClassRep.h"
#include "scriptEngine/concreteClassRep.h"
#include "scriptEngine/scriptEngine.h"

#ifdef __APPLE__
#define stricmp strcasecmp
#else
//#define stricmp stricmp
#endif

ConcreteClassRep<ScriptObject> ScriptObject::sConcreteClassRep("ScriptObject", "");

ScriptObject::ScriptObject() {
	mClassRep = nullptr;
	mName = "";
}

ScriptObject::~ScriptObject() {

}

void ScriptObject::initFields() {
	sConcreteClassRep.addSimpleField<std::string>("name", offsetof(ScriptObject, mName));
}

bool ScriptObject::getMemberField(const std::string &name, std::string &value) {
	if (!getClassRep()->isField(name)) {
		//Not a member field
		return false;
	}

	//Get field offset/getter data from the classrep
	AbstractClassRep::Field field = getClassRep()->getField(name);
	ptrdiff_t offset = field.offset;
	AbstractClassRep::GetterFunction fn = field.getter;

	if (fn == nullptr) {
		//No getter
		return false;
	}

	//Use the getter function on the field's pointer
	value = fn(reinterpret_cast<U8 *>(this) + offset);
	return true;
}

bool ScriptObject::setMemberField(const std::string &name, const std::string &value) {
	if (!getClassRep()->isField(name)) {
		//Not a member field
		return false;
	}

	//Get field offset/setter data from the classrep
	AbstractClassRep::Field field = getClassRep()->getField(name);
	ptrdiff_t offset = field.offset;
	AbstractClassRep::SetterFunction fn = field.setter;

	if (fn == nullptr) {
		//No setter
		return false;
	}

	//Use the setter function on the field's pointer
	return fn(reinterpret_cast<U8 *>(this) + offset, value);
}

bool ScriptObject::getDynamicField(const std::string &name, std::string &value) {
	//Do we have it?
	if (mDynamicFieldList.find(name) == mDynamicFieldList.end())
		return false;

	value = mDynamicFieldList[name];
	return true;
}

bool ScriptObject::setDynamicField(const std::string &name, const std::string &value) {
	mDynamicFieldList[name] = value;
	//Pretty much accept anything because dynamic
	return true;
}

bool ScriptObject::getField(const std::string &name, std::string &value) {
	//Try to use member fields first if we can
	if (getClassRep()->isField(name)) {
		//Don't try to make a dymanic field if this fails
		return getMemberField(name, value);
	}
	return getDynamicField(name, value);
}

bool ScriptObject::setField(const std::string &name, const std::string &value) {
	//Try to use member fields first if we can
	if (getClassRep()->isField(name)) {
		//Don't try to make a dymanic field if this fails
		return setMemberField(name, value);
	}
	return setDynamicField(name, value);
}