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

class ScriptObject {
public:
	ScriptObject();
	virtual ~ScriptObject();

	/*
	 * Adds a dynamic field to the instance of the object.
	 * @param key The field name.
	 * @param value The field value.
	 */
	void addDynamicField(const std::string &key, const std::string &value);

	/*
	 * Checks to see if the object contains a field with a case insensative key.
	 * @param key The field name.
	 * @return true if the field exists, false otherwise.
	 */
	bool containsField(const std::string &key);

protected:

	/*
	 * A list of fields that are extra, defined by the script, for the object.
	 * They are stored as strings.
	 */
	std::unordered_map<const char*, const char*> mDynamicFieldList;


};

#endif // _GAME_SCRIPTOBJECT_H_