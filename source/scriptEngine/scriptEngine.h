//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _SCRIPTENGINE_SCRIPTENGINE_H_
#define _SCRIPTENGINE_SCRIPTENGINE_H_

#include <vector>
#include "base/types.h"
#include "base/io.h"
#include <chaiscript/chaiscript.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>

class ScriptEngine {
public:
	enum Engine {
		SERVER,
		CLIENT
	};

	ScriptEngine();
	~ScriptEngine();

	static ScriptEngine* get(Engine isServer);

	bool init(const std::string &mainScript);

	bool execScript(const std::string &scriptFile);
private:
	chaiscript::ChaiScript *mEngine;
};

#include "scriptEngine/scriptEngine.inl"

#endif // _SCRIPTENGINE_SCRIPTENGINE_H_