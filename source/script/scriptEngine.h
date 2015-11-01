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

class ScriptClassConstructor {
public:
	struct Constructor {
		duk_c_function mConstructor;
		S32 mNumArgs;

		Constructor(duk_c_function constructor, S32 argc) : mConstructor(constructor), mNumArgs(argc) {}

		// Don't use
		Constructor() {}
	};

	struct Method {
		std::string mName;
		duk_c_function mFunction;
		S32 mNumArgs;

		Method(const std::string &name, duk_c_function function, S32 argc) : mName(name), mFunction(function), mNumArgs(argc) {}

		// Don't use
		Method() {}
	};

	ScriptClassConstructor(const std::string &name, const Constructor &constructor) {
		mName = name;
		mConstructor = constructor;
		mParentName = "";
	}

	void addMethod(const Method &method) {
		mMethods.push_back(method);
	}

	void setParent(const std::string &parent) {
		mParentName = parent;
	}

	ScriptClassConstructor *mNext;
	static ScriptClassConstructor *sLast;
private:
	std::string mName;
	std::string mParentName;
	std::vector<Method> mMethods;
	Constructor mConstructor;
};

/**
 * Checks the constructor to make sure that the required number of parameters
 * where met, as well as making sure that the constructor was called with the
 * new keyword from javascript.
 * @param context The heap context of the javascript virtual machine.
 * @param className The name of the class this contructor belongs to.
 * @param numArgs The number of arguments for the class's constructor.
 * @return 0 if success, DUK_RET_API_ERROR if new wasn't called, or 
 *  DUK_RET_RANGE_ERROR if the number of arguments didn't match.
 * @note FOR INTERNAL MACRO USE ONLY.
 */
static duk_ret_t __checkConstructor(duk_context *context, const char *className, S32 numArgs) {
	if (!duk_is_constructor_call(context)) {
		printf("You must call the constructor with new.\n"); 
		return DUK_RET_API_ERROR; 
	}

	// check for num args to make sure its right*
	if (duk_get_top(context) != numArgs) {
		printf("Call to constructor of class %s takes %i arguments. You passed %i arguments.\n", className, duk_get_top(context), numArgs);
		return DUK_RET_RANGE_ERROR;
	} 
	return 0;
}

/**
 * Finishes a constructor by doing some bookeeping work for the javascript-c++
 * bridge.
 * @param context The heap context of the javascript virtual machine.
 * @param function the c++ destructor function to be called whenever the garbage
 *  collector deletes this object.
 * @param instance The c++ pointer that the javascript object represents.
 * @note FOR INTERNAL MACRO USE ONLY.
 */
static void __finishConstructor(duk_context *context, duk_c_function function, void *instance) {
	duk_push_this(context);
	// Store handle ID 
	std::string handle = "___pointer";
	duk_push_pointer(context, instance);
	duk_put_prop_string(context, -2, handle.c_str());

	/* store destructor */
	duk_push_c_function(context, function, 1);
	duk_set_finalizer(context, -2);
}

/**
 * Implements a class and exposes it to script.
 */
#define IMPLEMENT_SCRIPT_OBJECT(klass, numArgs) \
	ScriptClassConstructor *klass::__scc##klass = new ScriptClassConstructor(##klass, ScriptClassConstructor::Constructor(##klass::__constructor##klass, numArgs));

 /**
  * The constructor that is run from javascript.
  * Put this inside of the public class definition.
  */
#define ScriptConstructor(klass, theConstructor, numArgs) \
	static ScriptClassConstructor *__scc##klass; \
	\
	static duk_ret_t __scriptDestruct##klass(duk_context *context) { \
		/* delete the object */ \
		duk_get_prop_string(context, 0, "___pointer"); \
		delete static_cast<klass *>(duk_to_pointer(context, -1)); \
		duk_pop(context); \
		return 0; \
	} \
	\
	/* Class constructor, called by javascript engine */ \
	static duk_ret_t __constructor##klass(duk_context *context) { \
		duk_ret_t check = __checkConstructor(context, #klass, numArgs); \
		if (check != 0) \
			return check; \
		\
		/* Handle our logic. */ \
		klass *instance = new klass theConstructor; \
		__constructorImplementation##klass(context, instance, numArgs); \
		\
		__finishConstructor(context, __scriptDestruct##klass, instance); \
		\
		return 0; \
	} \
	\
	static void __constructorImplementation##klass(duk_context *context, klass *object, S32 argc)

/**
 * Gets a number parameter from a specified argument index.
 * @param index The array index of the parameter list.
 * @note must be called in a script constructor.
 */
#define ScriptNumber(index) \
	duk_require_number(context, index)

 /**
 * Gets a string parameter from a specified argument index.
 * @param index The array index of the parameter list.
 * @note must be called in a script constructor.
 */
#define ScriptString(index) \
	duk_require_string(context, index)

//-----------------------------------------------------------------------------
// testing
//-----------------------------------------------------------------------------

class Point {
public:
	Point(F32 x, F32 y) : mX(x), mY(y) {}

	F32 mX;
	F32 mY;

	ScriptConstructor(Point, (0.0f, 0.0f), 2) {
		F32 x = static_cast<F32>(ScriptNumber(0));
		F32 y = static_cast<F32>(ScriptNumber(1));

		object->mX = x;
		object->mY = y;
	}
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