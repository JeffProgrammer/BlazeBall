//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _SCRIPTENGINE_CONCRETECLASSREP_H_
#define _SCRIPTENGINE_CONCRETECLASSREP_H_

#include <type_traits>
#include "scriptEngine/abstractClassRep.h"

/**
 * The main purpose of a ConcreteClassRep is to be able to instantiate the
 * actual instance of the object from the string provided to the
 * AbstractClassRep. All classes instantiated from ConcreteClassReps must
 * inherit from ScriptObject.
 */
template<typename ClassType>
class ConcreteClassRep : public AbstractClassRep {
public:
	/**
	 * Constructor of the ConcreteClassRep instance.
	 * @param name The name of the class that the ConcreteClassRep is 
	 *  instantiating.
	 */
	ConcreteClassRep(const std::string &name, const std::string &parent) {
		mName = name;
		mParent = parent;
	}

	/**
	 * Create an instance of the type.
	 * @return an instance of the type specified to the AbstractClassRep.
	 */
	virtual ScriptObject* create(World *world) {
		//For everything that is a subclass of GameObject, this will create it with
		// the world parameter. Everything else will be created without that parameter.
		ClassType *obj = new ClassType();
		obj->mClassRep = this;
		return obj;
	}

	virtual void initScript(ScriptEngine *engine) {
		ClassType::initScript(engine);
	}

	template<typename FieldType>
	void addSimpleField(ScriptEngine *scripting, FieldType ClassType:: *field, const std::string &name) {
		mFieldList[name] = Field(getOffset(field), scriptGetter<FieldType>, scriptSetter<FieldType>);
		scripting->addField(field, name);
	}
};

template <typename FieldType, typename Class>
inline ptrdiff_t getOffset(FieldType Class:: *field) {
	union {
		FieldType Class:: *m;
		ptrdiff_t ptr;
	} magicUnion;
	magicUnion.m = field;
	return magicUnion.ptr;
}

inline ptrdiff_t getOffset(long offset) {
	return static_cast<ptrdiff_t>(offset);
}

#define DECLARE_SCRIPTOBJECT(className) \
	static ConcreteClassRep<className> sConcreteClassRep

#define IMPLEMENT_SCRIPTOBJECT(className, parent) \
	ConcreteClassRep<className> className::sConcreteClassRep(#className, #parent)

#endif // _SCRIPTENGINE_CONCRETECLASSREP_H_