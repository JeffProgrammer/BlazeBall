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

void js_Point_initialize(duk_context *context);
duk_ret_t js_Point_destructor(duk_context *context); 
duk_ret_t js_Point_constructor(duk_context *context);
duk_ret_t js_Point_getX(duk_context *context);
duk_ret_t js_Point_setX(duk_context *context);

duk_ret_t js_Point_getY(duk_context *context);

class Point3 : public Point {
public:
	Point3(F32 x, F32 y, F32 z) : Point(x, y), mZ(z) {};

	F32 mZ;
};

void js_Point3_initialize(duk_context *context);
duk_ret_t js_Point3_destructor(duk_context *context);
duk_ret_t js_Point3_constructor(duk_context *context);
duk_ret_t js_Point3_getZ(duk_context *context);

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

