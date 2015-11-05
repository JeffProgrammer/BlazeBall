//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//
// ref: http://stackoverflow.com/questions/30296953/export-c-class-to-duktape
//------------------------------------------------------------------------------

#ifndef _SCRIPT_SCRIPTENGINE_H_
#define _SCRIPT_SCRIPTENGINE_H_

#include <duktape.h>
#include <string>
#include "base/types.h"
#include "script/scriptFunction.h"
#include "script/scriptClass.h"

#define SCRIPTENGINE ScriptEngine::getSingleton()

class ScriptEngine {
private:
	/**
	 * A pointer to the script engine interface context.
	 */
	duk_context *mContext;

	/**
	 * Initializes the class list, while recursivly checking for parent classes
	 * to ensure that parents are initialized before children. 
	 * @param alreadyInitialized a vector that holds all of the initialized
	 *  classes on the duktape heap.
	 * @param ssc The current ScriptClassConstructor to initialize on the heap.
	 */
	void initClasses(std::vector<ScriptClassConstructor*> &alreadyInitialized, ScriptClassConstructor *scc);

	/**
	 * Actually performs the initialization of the ScriptClassConstructor on the
	 * duktape heap.
	 * @param ssc The current ScriptClassConstructor to initialize on the heap.
	 */
	void initClassFinish(ScriptClassConstructor *constructor);

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
	std::string eval(const std::string &str);

	/**
	 * Executes a file script file and loads it into the virtual machine.
	 * @param file The script file to execute.
	 * @return the result of the script, or an empty string if there is
	 *  is no return value, or <Syntax Error> if there was a syntax error.
	 */
	std::string exec(const std::string &file);

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

//-----------------------------------------------------------------------------
// testing
//-----------------------------------------------------------------------------

class Point {
public:
	Point(F32 x, F32 y) : mX(x), mY(y) {}

	F32 mX;
	F32 mY;

	ScriptConstructor(Point, (0.0f, 0.0f), 2) {
		F32 x = ScriptParam::get<F32>(context, 0);
		F32 y = ScriptParam::get<F32>(context, 1);

		object->mX = x;
		object->mY = y;
	}
};

class Point3 : public Point {
public:
	Point3(F32 x, F32 y, F32 z) : Point(x, y), mZ(z) {};

	F32 mZ;

	ScriptConstructor(Point3, (0.0f, 0.0f, 0.0f), 3) {
		F32 x = ScriptParam::get<F32>(context, 0);
		F32 y = ScriptParam::get<F32>(context, 1);
		F32 z = ScriptParam::get<F32>(context, 2);

		object->mX = x;
		object->mY = y;
		object->mZ = z;
	}
};

#endif // _SCRIPT_SCRIPTENGINE_H_