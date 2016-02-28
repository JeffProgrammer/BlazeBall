//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _BEHAVIORS_BEHAVIORCONCRETECLASSREP_H_
#define _BEHAVIORS_BEHAVIORCONCRETECLASSREP_H_

#include "behaviors/behaviorAbstractClassRep.h"

template<typename ClassType>
class BehaviorConcreteClassRep : public BehaviorAbstractClassRep {
public:
	BehaviorConcreteClassRep(const std::string &name) {
		mName = name;
	}

	/**
	 * Create an instance of the type.
	 * @return an instance of the type specified to the BehaviorAbstractClassRep.
	 */
	virtual Behavior* create() {
		return new ClassType();
	}
};

#define DECLARE_BEHAVIOR(className) \
	static BehaviorConcreteClassRep<className> sBehaviorConcreteClassRep

#define IMPLEMENT_BEHAVIOR(className) \
	BehaviorConcreteClassRep<className> className::sConcreteClassRep(#className)

#endif // _BEHAVIORS_BEHAVIORCONCRETECLASSREP_H_