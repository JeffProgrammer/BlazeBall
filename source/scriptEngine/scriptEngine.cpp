//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "scriptEngine/scriptEngine.h"

#include <angelscript/add_on/scriptstdstring/scriptstdstring.h>
#include <angelscript/add_on/scripthandle/scripthandle.h>
#include <angelscript/add_on/scriptbuilder/scriptbuilder.h>
#include <angelscript/add_on/weakref/weakref.h>

ScriptEngine::ScriptEngine() {
	mEngine = nullptr;
	mCurrentContext = nullptr;
}

ScriptEngine::~ScriptEngine() {
	if (mEngine != nullptr)
		mEngine->Release();
}

void ScriptEngine::init() {
	// allocate the script engine
	mEngine = asCreateScriptEngine();

	// make std::string the type
	RegisterStdString(mEngine);

	// register handles
	RegisterScriptHandle(mEngine);

	// register weak refernces
	RegisterScriptWeakRef(mEngine);

	mCurrentContext = mEngine->CreateContext();
}

void ScriptEngine::prepareFunction(asIScriptFunction *function) {
	mCurrentContext->Prepare(function);
}

void ScriptEngine::executeFunction() {
	S32 code = mCurrentContext->Execute();
	if (code != asEXECUTION_FINISHED) {
		if (code == asEXECUTION_EXCEPTION) {
			// we got an exception
			printf("Exception: %s\n", mCurrentContext->GetExceptionString());
			printf("Function: %s\n", mCurrentContext->GetExceptionFunction()->GetDeclaration());
			printf("Line: %d\n", mCurrentContext->GetExceptionLineNumber());
		}
	}
}

void ScriptEngine::finishFunction(asIScriptContext *context) {
	context->Unprepare();
}