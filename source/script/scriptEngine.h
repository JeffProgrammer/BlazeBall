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

#ifdef _MSC_VER
#define FORCE_SCRIPT_INLINE __forceinline
#elif defined(__llvm__) || defined(__GNUC__)
#define FORCE_SCRIPT_INLINE inline __attribute__((always_inline))
#else
#define FORCE_SCRIPT_INLINE inline
#endif

#define SCRIPTENGINE ScriptEngine::getSingleton()

class ScriptClassConstructor;

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

		Method(const std::string &name, duk_c_function function, S32 argc, ScriptClassConstructor *constructor) : mName(name), mFunction(function), mNumArgs(argc) {
			constructor->addMethod(*this);
		}
	};

	struct Parent {
		Parent(const std::string &parentName, ScriptClassConstructor *parentConstructor) {
			parentConstructor->setParent(parentName);
		}
	};

	struct Field {
		std::string mName;
		duk_c_function mGetterFn;
		duk_c_function mSetterFn;

		Field(const std::string &name, duk_c_function getter, duk_c_function setter, ScriptClassConstructor *constructor) : mName(name), mGetterFn(getter), mSetterFn(setter) {
			constructor->addField(*this);
		}
	};

	ScriptClassConstructor(const std::string &name, const Constructor &constructor) {
		mName = name;
		mConstructor = constructor;
		mParentName = "";

		sScriptConstructorVector.push_back(this);
	}

	void addMethod(const Method &method) {
		mMethods.push_back(method);
	}

	void addField(const Field &field) {
		mFields.push_back(field);
	}

	void setParent(const std::string &parent) {
		mParentName = parent;
	}

	const std::string& getName() const {
		return mName;
	}

	const std::string& getParentName() const {
		return mParentName;
	}

	const std::vector<Method>& getMethods() const {
		return mMethods;
	}

	const std::vector<Field>& getFields() const {
		return mFields;
	}

	const Constructor& getConstructor() const {
		return mConstructor;
	}

	static std::vector<ScriptClassConstructor*> sScriptConstructorVector;
private:
	std::string mName;
	std::string mParentName;
	std::vector<Method> mMethods;
	std::vector<Field> mFields;
	Constructor mConstructor;
};

class ScriptFunctionConstructor {
public:
	ScriptFunctionConstructor(const std::string &name, S32 numArgs, duk_c_function function) : mName(name), mNumArgs(numArgs), mFunction(function) {
		sScriptFunctionConstructorVector.push_back(this);
	}

	std::string getName() const {
		return mName;
	}

	S32 getNumArgs() const {
		return mNumArgs;
	}

	duk_c_function getFunction() const {
		return mFunction;
	}

	static std::vector<ScriptFunctionConstructor*> sScriptFunctionConstructorVector;
private:
	std::string mName;
	S32 mNumArgs;
	duk_c_function mFunction;
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
FORCE_SCRIPT_INLINE duk_ret_t __checkConstructor(duk_context *context, const char *className, S32 numArgs) {
	if (!duk_is_constructor_call(context)) {
		printf("You must call the constructor with new.\n"); 
		return DUK_RET_API_ERROR; 
	}

	// check for num args to make sure its right
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
FORCE_SCRIPT_INLINE void __finishConstructor(duk_context *context, duk_c_function function, void *instance) {
	duk_push_this(context);
	// Store handle ID 
	std::string handle = "___pointer";
	duk_push_pointer(context, instance);
	duk_put_prop_string(context, -2, handle.c_str());

	/* store destructor */
	duk_push_c_function(context, function, 1);
	duk_set_finalizer(context, -2);
}

//Generic return
template<typename T>
struct ScriptFunctionReturn {
	static const int code = 1;
	typedef T(*Method)(duk_context *, S32);
	static int perform(duk_context *context, Method method, S32 numArgs) {
		T var = method(context, numArgs);
		duk_pop(context);
		pushVar(context, var);
		return code;
	}

	static void pushVar(duk_context *context, T var);
};
//Void-only return, doesn't try to get the retval
template<>
struct ScriptFunctionReturn<void> {
	static const int code = 0;
	typedef void(*Method)(duk_context *, S32);
	static int perform(duk_context *context, Method method, S32 numArgs) {
		method(context, numArgs);
		duk_pop(context);
		return code;
	}
};

//Specialized pushVar() implementations
template<>
inline void ScriptFunctionReturn<S32>::pushVar(duk_context *context, S32 var) {
	duk_push_int(context, var);
}
template<>
inline void ScriptFunctionReturn<F32>::pushVar(duk_context *context, F32 var) {
	duk_push_number(context, var);
}
template<>
inline void ScriptFunctionReturn<bool>::pushVar(duk_context *context, bool var) {
	duk_push_boolean(context, var);
}
template<>
inline void ScriptFunctionReturn<const char *>::pushVar(duk_context *context, const char *var) {
	duk_push_string(context, var);
}
#define ScriptFunction(name, rettype, numArgs)                                                                        \
	rettype __sfimplmenetation##name(duk_context *context, S32 argc);                                                  \
	static duk_ret_t __sf##name(duk_context *context) {                                                                \
		return ScriptFunctionReturn<rettype>::perform(context, __sfimplmenetation##name, numArgs);                      \
	}                                                                                                                  \
	ScriptFunctionConstructor __sfcfunctionconstructor##name(#name, numArgs, __sf##name);                              \
	rettype __sfimplmenetation##name(duk_context *context, S32 argc)

/**
 * Implements a class and exposes it to script.
 */
#define IMPLEMENT_SCRIPT_OBJECT(klass, numArgs)                                                                       \
	ScriptClassConstructor *klass::__scc##klass = new ScriptClassConstructor(#klass, ScriptClassConstructor::Constructor(klass::__constructor##klass, numArgs))

#define IMPLEMENT_SCRIPT_PARENT(klass, parent)                                                                        \
	ScriptClassConstructor::Parent __spp##klass##parent(#parent, klass::__scc##klass)

 /**
  * The constructor that is run from javascript.
  * Put this inside of the public class definition.
  */
#define ScriptConstructor(klass, theConstructor, numArgs)                                                             \
	static ScriptClassConstructor *__scc##klass;                                                                       \
	static duk_ret_t __scriptDestruct##klass(duk_context *context) {                                                   \
		/* delete the object */                                                                                         \
		duk_get_prop_string(context, 0, "___pointer");                                                                  \
		delete static_cast<klass *>(duk_to_pointer(context, -1));                                                       \
		duk_pop(context);                                                                                               \
		return 0;                                                                                                       \
	}                                                                                                                  \
	/* Class constructor, called by javascript engine */                                                               \
	static duk_ret_t __constructor##klass(duk_context *context) {                                                      \
		duk_ret_t check = __checkConstructor(context, #klass, numArgs);                                                 \
		if (check != 0)                                                                                                 \
			return check;                                                                                                \
		/* Handle our logic. */                                                                                         \
		klass *instance = new klass theConstructor;                                                                     \
		__constructorImplementation##klass(context, instance, numArgs);                                                 \
		__finishConstructor(context, __scriptDestruct##klass, instance);                                                \
		return 0;                                                                                                       \
	}                                                                                                                  \
	static void __constructorImplementation##klass(duk_context *context, klass *object, S32 argc)

// Generic return
template<typename T>
struct ScriptMethodReturn {
	static const int code = 1;

	template<typename K>
	static int perform(duk_context *context, K *object, T(*method)(duk_context*, K*, S32), S32 numArgs) {
		T var = method(context, object, numArgs);
		duk_pop(context);
		pushVar(context, var);
		return code;
	}

	static void pushVar(duk_context *context, T var);
};
// Void-only return, doesn't try to get the retval
template<>
struct ScriptMethodReturn<void> {
	static const int code = 0;

	template<typename K>
	static int perform(duk_context *context, K *object, void(*method)(duk_context*, K*, S32), S32 numArgs) {
		method(context, object, numArgs);
		duk_pop(context);
		return code;
	}
};

//Specialized pushVar() implementations
template<>
inline void ScriptMethodReturn<S32>::pushVar(duk_context *context, S32 var) {
	duk_push_int(context, var);
}
template<>
inline void ScriptMethodReturn<F32>::pushVar(duk_context *context, F32 var) {
	duk_push_number(context, var);
}
template<>
inline void ScriptMethodReturn<bool>::pushVar(duk_context *context, bool var) {
	duk_push_boolean(context, var);
}
template<>
inline void ScriptMethodReturn<const char *>::pushVar(duk_context *context, const char *var) {
	duk_push_string(context, var);
}

#define ScriptMethod(klass, name, rettype, numArgs)                                                                                                                     \
	rettype __methodImplementation##name##klass(duk_context *context, klass *object, S32 argc);                           \
	static duk_ret_t __method##name##klass(duk_context *context) {                                                     \
		duk_push_this(context);                                                                                         \
		duk_get_prop_string(context, -1, "___pointer");                                                                 \
		klass *object = static_cast<klass *>(duk_to_pointer(context, -1));                                              \
		return ScriptMethodReturn<rettype>::perform(context, object, __methodImplementation##name##klass, numArgs);     \
	}                                                                                                                  \
	ScriptClassConstructor::Method mc##klass##name(#name, __method##name##klass, numArgs, klass::__scc##klass);        \
	rettype __methodImplementation##name##klass(duk_context *context, klass *object, S32 argc)

#define ScriptField(klass, type, scriptname, fieldname, duktype)                                                      \
	static duk_ret_t __settermethod##scriptname##klass(duk_context *context) {                                         \
		duk_push_this(context);                                                                                         \
		duk_get_prop_string(context, -1, "___pointer");                                                                 \
		klass *object = static_cast<klass *>(duk_to_pointer(context, -1));                                              \
		duk_pop(context);                                                                                               \
		type x = static_cast<type>(duk_require_##duktype(context, 0));                                                  \
		object->fieldname = x;                                                                                          \
		return 0;                                                                                                       \
	}                                                                                                                  \
	static duk_ret_t __gettermethod##scriptname##klass(duk_context *context) {                                         \
		duk_push_this(context);                                                                                         \
		duk_get_prop_string(context, -1, "___pointer");                                                                 \
		klass *object = static_cast<klass *>(duk_to_pointer(context, -1));                                              \
		duk_pop(context);                                                                                               \
		duk_push_##duktype(context, object->fieldname);                                                                 \
		return 1;                                                                                                       \
	}                                                                                                                  \
	ScriptClassConstructor::Field __fgsc##scriptname##klass(#scriptname, __gettermethod##scriptname##klass, __settermethod##scriptname##klass, klass::__scc##klass)

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

#define ScriptReturnNumber(number) \
	duk_push_number(context, number)

#define ScriptReturnString(str) \
	duk_push_string(context, str)

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

class Point3 : public Point {
public:
	Point3(F32 x, F32 y, F32 z) : Point(x, y), mZ(z) {};

	F32 mZ;

	ScriptConstructor(Point3, (0.0f, 0.0f, 0.0f), 3) {
		F32 x = static_cast<F32>(ScriptNumber(0));
		F32 y = static_cast<F32>(ScriptNumber(1));
		F32 z = static_cast<F32>(ScriptNumber(2));

		object->mX = x;
		object->mY = y;
		object->mZ = z;
	}
};