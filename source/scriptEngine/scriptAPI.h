//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _SCRIPTENGINE_SCRIPTAPI_H_
#define _SCRIPTENGINE_SCRIPTAPI_H_

#include "scriptEngine/scriptEngine.h"

class ScriptConsoleFunction {
public:
	ScriptConsoleFunction(const std::string name, const chaiscript::Proxy_Function &fn) {
		mNext = sLast;
		sLast = this;

		mName = name;
		mFunction = fn;
	}

	ScriptConsoleFunction *mNext;
	static ScriptConsoleFunction *sLast;

	const std::string getName() const {
		return mName;
	}

	const chaiscript::Proxy_Function& getFunction() const {
		return mFunction;
	}

private:
	chaiscript::Proxy_Function mFunction;
	std::string mName;
};

#define BindScriptFunction(name, function) \
	ScriptConsoleFunction scf_##name(#name, chaiscript::fun(&function))

#define BindTemplateScriptFunction(name, type, function) \
	ScriptConsoleFunction scf_##name##type(#name, chaiscript::fun(&function))

#endif // _SCRIPTENGINE_SCRIPTAPI_H_