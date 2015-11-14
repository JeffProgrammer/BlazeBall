//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "base/io.h"
#include "scriptEngine/scriptEngine.h"

#include <angelscript/add_on/scriptstdstring/scriptstdstring.h>
#include <angelscript/add_on/scripthandle/scripthandle.h>
#include <angelscript/add_on/scriptbuilder/scriptbuilder.h>
#include <angelscript/add_on/weakref/weakref.h>


static void ScriptMessageCallback(const asSMessageInfo *msg, void *param) {
	if (msg->type == asMSGTYPE_ERROR)
		printf("SCRIPT MESSAGE [ERROR]: %s, line: %d, %s\n", msg->section, msg->row, msg->message);
	else if (msg->type == asMSGTYPE_WARNING)
		printf("SCRIPT MESSAGE [WARN]: %s, line: %d, %s\n", msg->section, msg->row, msg->message);
	else if (msg->type == asMSGTYPE_INFORMATION)
		printf("SCRIPT MESSAGE [INFO]: %s, line: %d, %s\n", msg->section, msg->row, msg->message);
	else {
		printf("SCRIPT MESSAGE [UNKNOWN]\n");
		assert(false);
	}
}

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
	mEngine->SetMessageCallback(asFUNCTION(ScriptMessageCallback), 0, asCALL_CDECL);

	// make std::string the type
	RegisterStdString(mEngine);

	// register handles
	RegisterScriptHandle(mEngine);

	// register weak refernces
	RegisterScriptWeakRef(mEngine);

	mCurrentContext = mEngine->CreateContext();
}

bool ScriptEngine::compileScript(const std::string &scriptFile) {
	if (!IO::isfile(scriptFile)) {
		printf("Unable to find script file %s\n", scriptFile.c_str());
		return false;
	}

   const char *fileBase = IO::getBase(scriptFile).c_str();
   
   if (containsModule(fileBase))
      printf("Attempting to recreate module %s\n", fileBase);
   

	// Create a new script module for this file.
	CScriptBuilder scriptBuilder;
	if (!scriptBuilder.StartNewModule(mEngine, fileBase)) {
		printf("Unable to create scriptModule %s\n", fileBase);
		return false;
	}

	// Load script
	if (!scriptBuilder.AddSectionFromFile(scriptFile.c_str())) {
		printf("Unable to load script file %s\n", scriptFile.c_str());
		return false;
	}

	// Build the script
	if (!scriptBuilder.BuildModule()) {
		printf("Unable to build module %s\n", fileBase);
		return false;
	}

	// succeeded
	printf("Loading module %s successfully!\n", fileBase));
	return true;
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

bool ScriptEngine::containsModule(const char *module) {
   U32 count = mEngine->GetModuleCount();
   for (U32 i = 0; i < count; i++) {
      if (strcmp(mEngine->GetModuleByIndex(i)->GetName(), module) == 0)
         return true;
   }
   return false;
}