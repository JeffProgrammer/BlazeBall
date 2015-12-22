//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GAME_SCRIPTOBJECT_H_
#define _GAME_SCRIPTOBJECT_H_

#include "base/types.h"
#include <string>
#include <unordered_map>

class AbstractClassRep;
template<typename T> class ConcreteClassRep;

class ScriptObject {
	template<typename T>
	friend class ConcreteClassRep;
public:
	ScriptObject();
	virtual ~ScriptObject();

	static void initFields();

	virtual AbstractClassRep* getClassRep() {
		return mClassRep;
	}

	bool getField(const std::string &name, std::string &value);
	bool setField(const std::string &name, const std::string &value);

protected:

	bool getDynamicField(const std::string &name, std::string &value);
	bool setDynamicField(const std::string &name, const std::string &value);

	bool getMemberField(const std::string &name, std::string &value);
	bool setMemberField(const std::string &name, const std::string &value);

	std::string mName;
	U32 mId;

	/*
	 * A list of fields that are extra, defined by the script, for the object.
	 * They are stored as strings.
	 */
	std::unordered_map<std::string, std::string> mDynamicFieldList;

	AbstractClassRep *mClassRep;

	static ConcreteClassRep<ScriptObject> sConcreteClassRep;
};

#endif // _GAME_SCRIPTOBJECT_H_