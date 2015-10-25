//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include <duktape.h>
#include <string>
#include "base/types.h"

#define SCRIPTENGINE ScriptEngine::getSingleton()

class ScriptEngine {
private:
	duk_context *mContext;

public:
	ScriptEngine();
	~ScriptEngine();

	void init();

	std::string eval(const char *str);

	static ScriptEngine *getSingleton() {
		static ScriptEngine *engine = new ScriptEngine();
		return engine;
	}

	inline void destroy() {
		delete this;
	}
};

