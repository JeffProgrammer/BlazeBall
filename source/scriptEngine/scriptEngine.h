//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _SCRIPTENGINE_SCRIPTENGINE_H_
#define _SCRIPTENGINE_SCRIPTENGINE_H_

#include <vector>
#include <chaiscript/chaiscript.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>
#include "base/types.h"
#include "base/io.h"

namespace chaiscript {
	class ChaiScript;
}

class ScriptEngine {
public:
	ScriptEngine();
	~ScriptEngine();

	bool init(const std::string &mainScript);

	bool execScript(const std::string &scriptFile);

	template<typename Method>
	void addMethod(Method method, const std::string &name) {
		mEngine->add(chaiscript::fun(method), name);
	}

	template<typename Field>
	void addField(Field field, const std::string &name) {
		mEngine->add(chaiscript::fun(field), name);
	}

	template<typename Class>
	void addClass(const std::string &className) {
		mEngine->add(chaiscript::user_type<Class>(), className);
		mEngine->add(chaiscript::constructor<Class()>(), className);
	}

	template<typename Class, typename Parent, bool isConstructable = true>
	typename std::enable_if<isConstructable, void>::type addClass(const std::string &className) {
		mEngine->add(chaiscript::user_type<Class>(), className);
		mEngine->add(chaiscript::constructor<Class()>(), className);
		mEngine->add(chaiscript::base_class<Parent, Class>());
	}

	template<typename Class, typename Parent, bool isConstructable = true>
	typename std::enable_if<!isConstructable, void>::type addClass(const std::string &className) {
		mEngine->add(chaiscript::user_type<Class>(), className);
		mEngine->add(chaiscript::base_class<Parent, Class>());
	}
private:
	chaiscript::ChaiScript *mEngine;
};

#include "scriptEngine/scriptEngine.inl"

#endif // _SCRIPTENGINE_SCRIPTENGINE_H_