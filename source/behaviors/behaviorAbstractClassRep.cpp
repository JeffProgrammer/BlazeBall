//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "behaviors/behaviorAbstractClassRep.h"

// initialize the lined list for abstractclassrep
BehaviorAbstractClassRep *BehaviorAbstractClassRep::sLast = nullptr;

Behavior* BehaviorAbstractClassRep::createFromName(const std::string &name) {
	for (auto *rep = sLast; rep != nullptr; rep = rep->mNext) {
		if (rep->mName == name)
			return rep->create();
	}
	return nullptr;
}