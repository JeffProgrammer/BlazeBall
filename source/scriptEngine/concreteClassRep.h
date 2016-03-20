//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the project nor the
//      names of its contributors may be used to endorse or promote products
//      derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

	virtual void initFields() {
		ClassType::initFields();
	}

	template<typename FieldType>
	void addSimpleField(FieldType ClassType:: *field, const std::string &name) {
		mFieldList[name] = Field(getOffset(field), scriptGetter<FieldType>, scriptSetter<FieldType>);
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

#define AddField(field, name) \
	sConcreteClassRep.addSimpleField(&field, name)

#endif // _SCRIPTENGINE_CONCRETECLASSREP_H_