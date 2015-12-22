//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _SCRIPTENGINE_CONCRETECLASSREP_H_
#define _SCRIPTENGINE_CONCRETECLASSREP_H_

#include "scriptEngine/abstractClassRep.h"

/**
 * The main purpose of a ConcreteClassRep is to be able to instantiate the
 * actual instance of the object from the string provided to the
 * AbstractClassRep. All classes instantiated from ConcreteClassReps must
 * inherit from ScriptObject.
 */
template<typename T>
class ConcreteClassRep : public AbstractClassRep {
public:
	/**
	 * Constructor of the ConcreteClassRep instance.
	 * @param name The name of the class that the ConcreteClassRep is 
	 *  instantiating.
	 */
	ConcreteClassRep(const std::string &name) {
		mName = name;
	}

	/**
	 * Create an instance of the type.
	 * @return an instance of the type specified to the AbstractClassRep.
	 */
	virtual ScriptObject* create() {
		T *obj = new T();
		obj->mClassRep = this;
		return obj;
	}

	virtual void initClass() {
		T::initFields();
	}
};

#endif // _SCRIPTENGINE_CONCRETECLASSREP_H_