//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _SCRIPTENGINE_ABSTRACTCLASSREP_H_
#define _SCRIPTENGINE_ABSTRACTCLASSREP_H_

#include <unordered_map>
#include <string>
#include "game/scriptObject.h"

class AbstractClassRep {
public:
	AbstractClassRep() {

	}

	virtual ~AbstractClassRep() {

	}

	virtual ScriptObject* create() = 0;

	static ScriptObject* createFromName(const std::string &name) {
		return sClassRepMap[name]->create();
	}

private:
	static std::unordered_map<std::string, AbstractClassRep*> sClassRepMap;
};

#endif // _SCRIPTENGINE_ABSTRACTCLASSREP_H_