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
duk_ret_t js_Point_setY(duk_context *context);

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
	/**
	 * A pointer to the script engine interface context.
	 */
	duk_context *mContext;

public:
	ScriptEngine();
	~ScriptEngine();

	/**
	 * Initializes the script engine.
	 */
	void init();

	/**
	 * Evaluates an arbitrary line of code.
	 * @param str The code to evaluate.
	 * @return the result of the script, or an empty string if there is no 
	 *  return value or undefined.
	 */
	std::string eval(const char *str);

	/**
	 * Executes a file script file and loads it into the virtual machine.
	 * @param file The script file to execute.
	 * @return the result of the script, or an empty string if there is
	 *  is no return value, or <Syntax Error> if there was a syntax error.
	 */
	std::string exec(const char *file);

	/**
	 * Gets the script engine instance.
	 * @return a pointer to the singleton script engine.
	 */
	static ScriptEngine *getSingleton() {
		static ScriptEngine *engine = new ScriptEngine();
		return engine;
	}

	/**
	 * Destroys the script engine.
	 */
	inline void destroy() {
		delete this;
	}
};

