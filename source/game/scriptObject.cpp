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

#include "game/scriptObject.h"
#include "scriptEngine/abstractClassRep.h"
#include "scriptEngine/concreteClassRep.h"
#include "behaviors/behavior.h"
#include "game/world.h"

#ifdef __APPLE__
#define stricmp strcasecmp
#else
//#define stricmp stricmp
#endif

ConcreteClassRep<ScriptObject> ScriptObject::sConcreteClassRep("ScriptObject", "");

ScriptObject::ScriptObject() {
	mClassRep = nullptr;
	mWorld = nullptr;
	mName = "";
	mBehaviorString = "";

	IO::printf("Constructed object %p\n", this);
}

ScriptObject::~ScriptObject() {
	// Cleanup is called here on all behaviors
	for (auto *behavior : mBehaviors)
		behavior->cleanup();

	IO::printf("Destructed object %p\n", this);
}

void ScriptObject::initFields() {
	AddField(ScriptObject::mName, "name");
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

void ScriptObject::update(const F64 &dt) {
	for (auto behavior : mBehaviors)
		behavior->update(dt);
}