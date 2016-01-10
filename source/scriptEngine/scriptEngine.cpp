//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "scriptEngine/scriptEngine.h"
#include "scriptEngine/abstractClassRep.h"
#include "scriptEngine/scriptFunctions.h"

// initialize the lined list for abstractclassrep
AbstractClassRep *AbstractClassRep::sLast = nullptr;
std::unordered_map<std::string, AbstractClassRep*> AbstractClassRep::sClassRepMap;

ScriptEngine::ScriptEngine() {

}

ScriptEngine::~ScriptEngine() {

}

bool ScriptEngine::init() {
	// init engine
	mEngine = new chaiscript::ChaiScript(chaiscript::Std_Lib::library());

	mEngine->add(chaiscript::fun(&echo), "echo");

	// execute main.chai
	execScript("main.chai");
	return true;
}

bool ScriptEngine::execScript(const std::string &scriptFile) {
	mEngine->eval_file(scriptFile);
	return true;
}
