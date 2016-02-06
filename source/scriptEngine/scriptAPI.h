//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _SCRIPTENGINE_SCRIPTAPI_H_
#define _SCRIPTENGINE_SCRIPTAPI_H_

#include "scriptEngine/scriptEngine.h"

/**
 * Internal class that holds information about global namespace functions that
 * are exposed to the scripting engine.
 *
 * The class itself is for internal usage only. The public API should be the
 * macros provided down below.
 *
 * To expose a normal, non-templated function, use the following macro:
 *    BindScriptFunction(function_api_here_in_script, function_c++_name_here);
 * Example:
 * 
 *   int add_fn(int a, int b) { return a + b; }
 *   BindScriptFunction(add, add_fn);
 *
 * To expose a templated or overloaded function, use the following macro:
 *    BindOverloadedScriptFunction(function_api_here_in_script,
 *                                 one_different_type_here_used_internally,
 *                                 function_c++_name_here);
 * Template exmaple:
 *
 *    template<typename T>
 *    static void echo(const T &in) { IO::printf("%s\n", std::to_string(in).c_str()); }
 *    template<>
 *    static void echo(const std::string &in) { IO::printf("%s\n", in.c_str()); }
 *    BindOverloadedScriptFunction(echo, string, echo<std::string>);
 *    BindOverloadedScriptFunction(echo, F32, echo<F32>);
 *    BindOverloadedScriptFunction(echo, S32, echo<S32>);
 */
class ScriptConsoleFunction {
public:
	/**
	 * Constructs an object of type ScriptConsoleFunction.
	 * @param name The script API name of the function.
	 * @param fn The function prototype of the function that hooks into the
	 * script engine.
	 * @return The constructed ScriptConsoleFunction object.
	 */
	ScriptConsoleFunction(const std::string name, const chaiscript::Proxy_Function &fn) {
		mNext = sLast;
		sLast = this;

		mName = name;
		mFunction = fn;
	}

	/**
	 * The next function object to iterate through the linked list, or nullptr.
	 */
	ScriptConsoleFunction *mNext;

	/**
	 * Holds a pointer the linked list starting at the last item in the list 
	 * added. The last node will have nullptr for the next value.
	 */
	static ScriptConsoleFunction *sLast;

	/**
	 * Gets the script API name of the function that is to be called in script.
	 * @return The script API name of the function.
	 */
	const std::string getName() const {
		return mName;
	}

	/**
	 * Gets the script engine function prototype.
	 * @return The function prototype for the script engine.
	 */
	const chaiscript::Proxy_Function& getFunction() const {
		return mFunction;
	}

private:
	/**
	 * The function prototype for the script engine.
	 */
	chaiscript::Proxy_Function mFunction;

	/**
	 * The script API name for the function.
	 */
	std::string mName;
};

/**
 * Binds a non-overloaded/non-templated c++ function to the script layer.
 * @param name The script API name of the function.
 * @param function The c++ function.
 */
#define BindScriptFunction(name, function) \
	ScriptConsoleFunction scf_##name(#name, chaiscript::fun(&function))

/**
 * Binds a overloaded or templated c++ function to the script layer.
 * @param name The script API name of the function.
 * @param type The specialized type of the function. Note that this
 *  does not affect the scripting layer at all, as this is just used
 *  internally to use as part of the unique object name. Since it is
 *  overloaded, we have to have some way of specifying a unique
 *  object.
 * @param function The c++ function.
 */
#define BindOverloadedScriptFunction(name, type, function) \
	ScriptConsoleFunction scf_##name##type(#name, chaiscript::fun(&function))

#endif // _SCRIPTENGINE_SCRIPTAPI_H_