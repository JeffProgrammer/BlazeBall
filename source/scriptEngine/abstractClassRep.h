//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _SCRIPTENGINE_ABSTRACTCLASSREP_H_
#define _SCRIPTENGINE_ABSTRACTCLASSREP_H_

#include <unordered_map>
#include <string>
#include "base/Tree.h"
#include "game/scriptObject.h"
#include "scriptEngine/scriptEngine.h"

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

	template<typename T>
	void addSimpleField(const std::string &name, ptrdiff_t offset) {
		mFieldList[name] = Field(offset, scriptGetter<T>, scriptSetter<T>);
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
	virtual void initScript(ScriptEngine *engine) = 0;

	std::string toString() {
		return mName;
	}

	std::string getName() const {
		return mName;
	}

	static ScriptObject* createFromName(const std::string &name, World *world = nullptr);
	static void initScriptAPI(ScriptEngine *engine);

protected:
	static std::unordered_map<std::string, AbstractClassRep*> sClassRepMap;
	static AbstractClassRep *sLast;
	AbstractClassRep *mNext;

	std::unordered_map<std::string, Field> mFieldList;
	std::string mName;
	std::string mParent;
};

#endif // _SCRIPTENGINE_ABSTRACTCLASSREP_H_