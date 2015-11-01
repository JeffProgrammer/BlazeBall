//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//
// ref: http://stackoverflow.com/questions/30296953/export-c-class-to-duktape
//------------------------------------------------------------------------------

#ifndef _SCRIPT_SCRIPTFUNCTION_H_
#define _SCRIPT_SCRIPTFUNCTION_H_

#include "script/scriptTemplates.h"

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

#define ScriptFunction(name, rettype, numArgs)                                                                        \
	rettype __sfimplmenetation##name(duk_context *context, S32 argc);                                                  \
	static duk_ret_t __sf##name(duk_context *context) {                                                                \
		return ScriptFunctionReturn<rettype>::perform(context, __sfimplmenetation##name, numArgs);                      \
	}                                                                                                                  \
	ScriptFunctionConstructor __sfcfunctionconstructor##name(#name, numArgs, __sf##name);                              \
	rettype __sfimplmenetation##name(duk_context *context, S32 argc)


#endif // _SCRIPT_SCRIPTFUNCTION_H_