//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _SCRIPTENGINE_ABSTRACTCLASSREP_H_
#define _SCRIPTENGINE_ABSTRACTCLASSREP_H_

#include <unordered_map>
#include <string>
#include "game/scriptObject.h"
#include "scriptEngine/scriptEngine.h"

class AbstractClassRep {
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

	template<typename T>
	void addSimpleField(const std::string &name, ptrdiff_t offset) {
		mFieldList[name] = Field(offset, scriptGetter<T>, scriptSetter<T>);
	}

	virtual ScriptObject* create() = 0;

	static ScriptObject* createFromName(const std::string &name) {
		return sClassRepMap[name]->create();
	}

	static void init() {
		for (auto *rep = sLast; rep != nullptr; rep = rep->mNext) {
			sClassRepMap[rep->mName] = rep;
			rep->initClass();
		}
	}

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

	Field getField(const std::string &name) {
		return mFieldList[name];
	}

	virtual void initClass() = 0;

protected:
	static std::unordered_map<std::string, AbstractClassRep*> sClassRepMap;
	static AbstractClassRep *sLast;
	AbstractClassRep *mNext;

	std::unordered_map<std::string, Field> mFieldList;
	std::string mName;
};

#endif // _SCRIPTENGINE_ABSTRACTCLASSREP_H_