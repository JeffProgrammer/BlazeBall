//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _SCRIPTENGINE_CONCRETECLASSREP_H_
#define _SCRIPTENGINE_CONCRETECLASSREP_H_

#include <type_traits>
#include "scriptEngine/abstractClassRep.h"

class GameObject;

//Nasty template hacking
template<bool=true>
struct create_impl {
	template<typename T>
	static T *create(World *world) {
		return new T(world);
	}
};

template<>
struct create_impl<false> {
	template<typename T>
	static T *create(World *world) {
		return new T();
	}
};

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
		T *obj = create_impl<std::is_base_of<GameObject, T>::value>::template create<T>(world);
		obj->mClassRep = this;
		return obj;
	}

	virtual void initClass() {
		// copy the parent's fields
		// this will perform a deep copy of all fields since we call initClass on
		// an ordered tree!
		if (mParent != "") {
			const auto &fields = sClassRepMap[mParent]->mFieldList;
			mFieldList.insert(fields.begin(), fields.end());
		}
		T::initFields();
	}

	virtual void initScript(ScriptEngine *engine) {
		T::initScript(engine);
	}
};

#define DECLARE_SCRIPTOBJECT(className) \
	static ConcreteClassRep<className> sConcreteClassRep

#define IMPLEMENT_SCRIPTOBJECT(className, parent) \
	ConcreteClassRep<className> className::sConcreteClassRep(#className, #parent)

#define AddFieldSimple(name, type, offset) \
	sConcreteClassRep.addSimpleField<type>(name, offset)

#endif // _SCRIPTENGINE_CONCRETECLASSREP_H_