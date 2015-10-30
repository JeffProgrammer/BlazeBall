//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//
// ref: http://stackoverflow.com/questions/30296953/export-c-class-to-duktape
//------------------------------------------------------------------------------

#include <duktape.h>
#include <string>
#include "base/types.h"

#define SCRIPTENGINE ScriptEngine::getSingleton()

class Point {
public:
	Point(F32 x, F32 y) : mX(x), mY(y) {}

	F32 mX;
	F32 mY;
};

duk_ret_t js_Point_destructor(duk_context *context); 
duk_ret_t js_Point_constructor(duk_context *context);
duk_ret_t js_Point_getX(duk_context *context);

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

