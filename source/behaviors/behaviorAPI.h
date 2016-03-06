//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _BEHAVIORS_BEHAVIORAPI_H_
#define _BEHAVIORS_BEHAVIORAPI_H_

#include "behaviors/behaviorConcreteClassRep.h"

#define DECLARE_BEHAVIOR(className) \
	static BehaviorConcreteClassRep<className> sBehaviorConcreteClassRep

#define IMPLEMENT_BEHAVIOR(className) \
	BehaviorConcreteClassRep<className> className::sBehaviorConcreteClassRep(#className)

#endif // _BEHAVIORS_BEHAVIORAPI_H_