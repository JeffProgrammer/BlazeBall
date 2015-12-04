//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "base/io.h"
#include "scriptEngine/scriptEngine.h"
#include "scriptEngine/scriptFunctions.h"

#include <angelscript/add_on/scriptstdstring/scriptstdstring.h>
#include <angelscript/add_on/scripthandle/scripthandle.h>
#include <angelscript/add_on/scriptbuilder/scriptbuilder.h>
#include <angelscript/add_on/scriptmath/scriptmath.h>
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

bool ScriptEngine::init() {
	if (mEngine != nullptr) {
		printf("ScriptEngine::init() - Already called init.\n");
		return false;
	}

	// allocate the script engine
	mEngine = asCreateScriptEngine();
	mEngine->SetMessageCallback(asFUNCTION(ScriptMessageCallback), 0, asCALL_CDECL);

	// make std::string the type
	RegisterStdString(mEngine);

	// register handles
	RegisterScriptHandle(mEngine);

	// register weak refernces
	RegisterScriptWeakRef(mEngine);

	// register basic math functions
	RegisterScriptMath(mEngine);

	// Here we register global functions, classes, properties, methods, global variables, ect.
	{
		S32 r;
		r = mEngine->RegisterGlobalFunction("void echo(string &in)", asFUNCTION(echo), asCALL_CDECL); assert(r >= 0);
	}

	mCurrentContext = mEngine->CreateContext();

	// Load main.as and call the main function.
	if (!compileScript("main.as"))
		return false;

	asIScriptFunction *mainFn = mEngine->GetModule("main")->GetFunctionByDecl("void main()");
	if (mainFn == NULL) {
		printf("Unable to find function main.\n");
		return false;
	}

	// call the main function
	prepareFunction(mainFn);
	executeFunction<void>();

	// call test function to get a value
	asIScriptFunction *fn = mEngine->GetModule("main")->GetFunctionByDecl("float get5()");
	if (fn == NULL) {
		printf("Unable to find function get5\n");
		return false;
	}
	prepareFunction(fn);
	F32 val = executeFunction<float>();
	printf("get5() returned %f\n", val);

	// test add function and get value
	asIScriptFunction *addFn = mEngine->GetModule("main")->GetFunctionByDecl("float add(float a, float b)");
	if (addFn == NULL) {
		printf("Unable to find function add\n");
		return false;
	}
	prepareFunction(addFn);
	setParameter(0, 2.0f);
	setParameter(1, 1.0f);
	F32 addResult = executeFunction<float>();
	printf("add() returned %f\n", addResult);

	return true;
}

bool ScriptEngine::compileScript(const std::string &scriptFile) {
	if (!IO::isfile(scriptFile)) {
		printf("Unable to find script file %s\n", scriptFile.c_str());
		return false;
	}

	std::string base = IO::getBase(scriptFile);
	const char *fileBase = base.c_str();
   
	if (containsModule(fileBase))
		printf("Attempting to recreate module %s\n", fileBase);
   

	// Create a new script module for this file.
	CScriptBuilder scriptBuilder;
	if (scriptBuilder.StartNewModule(mEngine, fileBase) < 0) {
		printf("Unable to create scriptModule %s\n", fileBase);
		return false;
	}

	// Load script
	if (scriptBuilder.AddSectionFromFile(scriptFile.c_str()) < 0) {
		printf("Unable to load script file %s\n", scriptFile.c_str());
		return false;
	}

	// Build the script
	if (scriptBuilder.BuildModule() < 0) {
		printf("Unable to build module %s\n", fileBase);
		return false;
	}

	// succeeded
	printf("Loading module %s successfully!\n", fileBase);
	return true;
}

void ScriptEngine::prepareFunction(asIScriptFunction *function) {
	mCurrentContext->Prepare(function);
}

bool ScriptEngine::containsModule(const char *module) {
	U32 count = mEngine->GetModuleCount();
	for (U32 i = 0; i < count; i++) {
		if (strcmp(mEngine->GetModuleByIndex(i)->GetName(), module) == 0)
			return true;
	}
	return false;
}