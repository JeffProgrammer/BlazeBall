//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
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