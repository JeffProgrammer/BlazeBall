//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _SCRIPTENGINE_SCRIPTENGINE_H_
#define _SCRIPTENGINE_SCRIPTENGINE_H_

#include "angelscript.h"
#include "base/types.h"
#include <vector>

class ScriptEngine {
public:
	ScriptEngine();
	~ScriptEngine();

	void init();

	void prepareFunction(asIScriptFunction *function);
	void executeFunction();

	bool compileScript(const std::string &scriptFile);

	static ScriptEngine *getSingleton() {
		ScriptEngine *singleton = new ScriptEngine();
		return singleton;
	}

private:
	void finishFunction(asIScriptContext *context);


	asIScriptEngine *mEngine;

	asIScriptContext *mCurrentContext;
};

#endif // _SCRIPTENGINE_SCRIPTENGINE_H_