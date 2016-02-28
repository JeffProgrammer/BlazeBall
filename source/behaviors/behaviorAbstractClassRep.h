//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _BEHAVIORS_BEHAVIORABSTRACTCLASSREP_H_
#define _BEHAVIORS_BEHAVIORABSTRACTCLASSREP_H_

#include <string>
#include "behaviors/behavior.h"

class BehaviorAbstractClassRep {
	// This is fucking bullshit.
	// Let them access our private members :^)
	template<typename T> friend class BehaviorConcreteClassRep;
public:
	BehaviorAbstractClassRep() {
		mNext = sLast;
		sLast = this;
	}

	virtual Behavior* create() = 0;

	static Behavior* createFromName(const std::string &name);
protected:
	static BehaviorAbstractClassRep *sLast;
	BehaviorAbstractClassRep *mNext;

	std::string mName;
};

#endif // _BEHAVIORS_BEHAVIORABSTRACTCLASSREP_H_