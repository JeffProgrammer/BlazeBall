//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _SCRIPTENGINE_CONCRETECLASSREP_H_
#define _SCRIPTENGINE_CONCRETECLASSREP_H_

#include "scriptEngine/abstractClassRep.h"

template<typename T>
class ConcreteClassRep : public AbstractClassRep {
public:
	ConcreteClassRep(const std::string &name) {
		mName = name;
	}

	virtual ~ConcreteClassRep() {

	}

	virtual ScriptObject* create() {
		return new T();
	}

private:

};

#endif // _SCRIPTENGINE_CONCRETECLASSREP_H_