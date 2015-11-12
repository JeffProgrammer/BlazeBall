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
	// used for getting error codes from AngelScript
	S32 ret;

	if (!IO::isfile(scriptFile)) {
		printf("Unable to find script file %s\n", scriptFile.c_str());
		return false;
	}

	// read the contents of the file
	U32 fileLength = 0;
	U8 *fileContents = IO::readFile(scriptFile, fileLength);
	if (fileContents == NULL) {
		printf("Unable to read the script file %s\n", scriptFile.c_str());
		return false;
	}

	// TODO: make this per script. atm this will only work for 1 script file
	// Add the script into a newly created module.
	asIScriptModule *module = mEngine->GetModule(0, asGM_ALWAYS_CREATE);
	ret = module->AddScriptSection("script", reinterpret_cast<const char*>(fileContents), fileLength);
	if (ret < 0) {
		printf("Unable to add the script to %s\n", scriptFile.c_str());
		return false;
	}

	// compile the script inside of the AngelScript VM
	ret = module->Build();
	if (ret < 0) {
		printf("Script failed to build! %s\n", scriptFile.c_str());
		return false;
	}

	// free script contents
	delete[] fileContents;
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