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

#ifndef _GAME_SCRIPTOBJECT_H_
#define _GAME_SCRIPTOBJECT_H_

#include "base/types.h"
#include <string>
#include <unordered_map>
#include <memory>

class AbstractClassRep;
template<typename T> class ConcreteClassRep;
class ScriptObject;
class Behavior;
class World;

class ScriptObject {
	template<typename T>
	friend class ConcreteClassRep;
	friend class AbstractClassRep;
public:
	ScriptObject();
	virtual ~ScriptObject();

	static void initFields();

	virtual AbstractClassRep* getClassRep() {
		return mClassRep;
	}

	bool getField(const std::string &name, std::string &value);
	bool setField(const std::string &name, const std::string &value);

	void setName(const std::string &name) {
		mName = name;
	}

	inline const std::string getName() const {
		return mName;
	}

	inline void addBehavior(Behavior *behavior) {
		mBehaviors.push_back(behavior);
	}

	std::vector<Behavior*> getBehaviors() {
		return mBehaviors;
	}

	std::string getBehaviorString() {
		return mBehaviorString;
	}

	void setBehaviorString(const std::string &behaviors) {
		mBehaviorString = behaviors;
	}

	virtual void update(const F64 &dt);

	std::string mName;

protected:

	bool getDynamicField(const std::string &name, std::string &value);
	bool setDynamicField(const std::string &name, const std::string &value);

	bool getMemberField(const std::string &name, std::string &value);
	bool setMemberField(const std::string &name, const std::string &value);

	/*
	 * A list of fields that are extra, defined by the script, for the object.
	 * They are stored as strings.
	 */
	std::unordered_map<std::string, std::string> mDynamicFieldList;
	AbstractClassRep *mClassRep;

	World *mWorld;

	std::vector<Behavior*> mBehaviors;
	std::string mBehaviorString;

	static ConcreteClassRep<ScriptObject> sConcreteClassRep;
	
};

#endif // _GAME_SCRIPTOBJECT_H_