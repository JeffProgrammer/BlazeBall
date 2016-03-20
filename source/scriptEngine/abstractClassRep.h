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

#ifndef _SCRIPTENGINE_ABSTRACTCLASSREP_H_
#define _SCRIPTENGINE_ABSTRACTCLASSREP_H_

#include <unordered_map>
#include <string>
#include "base/Tree.h"
#include "game/scriptObject.h"

#include "scriptEngine/scriptTypes.inl"

class World;
class AbstractClassRep {
	// This is fucking bullshit.
	// Let them access our private members :^)
	template<typename T> friend class ConcreteClassRep;
public:
	typedef std::string (*GetterFunction)(void *ptr);
	typedef bool(*SetterFunction)(void *ptr, const std::string &value);

	AbstractClassRep() {
		mNext = sLast;
		sLast = this;
	}

	virtual ~AbstractClassRep() {

	}

	void addField(const std::string &name, ptrdiff_t offset, GetterFunction getterFunction, SetterFunction setterFunction) {
		mFieldList[name] = Field(offset, getterFunction, setterFunction);
	}

	template<typename ClassType, typename FieldType>
	void addSimpleField(FieldType ClassType:: *field, const std::string &name) {
		mFieldList[name] = Field(getOffset(field), scriptGetter<FieldType>, scriptSetter<FieldType>);
	}

	virtual ScriptObject* create(World *world) = 0;
	struct Field {
		Field() {
			offset = 0;
			getter = nullptr;
			setter = nullptr;
		}

		Field(ptrdiff_t off, GetterFunction g, SetterFunction s) {
			offset = off;
			getter = g;
			setter = s;
		}

		ptrdiff_t offset;

		GetterFunction getter;
		SetterFunction setter;
	};

	bool isField(const std::string &name) {
		return mFieldList.find(name) != mFieldList.end();
	}

	Field getField(const std::string &name) {
		return mFieldList[name];
	}

	virtual void initFields() = 0;

	std::string toString() {
		return mName;
	}

	std::string getName() const {
		return mName;
	}

	static ScriptObject* createFromName(const std::string &name, World *world = nullptr);
	static void init();

protected:
	static std::unordered_map<std::string, AbstractClassRep*> sClassRepMap;
	static AbstractClassRep *sLast;
	AbstractClassRep *mNext;

	std::unordered_map<std::string, Field> mFieldList;
	std::string mName;
	std::string mParent;
};

#endif // _SCRIPTENGINE_ABSTRACTCLASSREP_H_