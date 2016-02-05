//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "scriptEngine/scriptEngine.h"

#include <glm/gtx/string_cast.hpp>

#include "scriptEngine/abstractClassRep.h"
#include "scriptEngine/scriptFunctions.h"
#include "scriptEngine/scriptMath.h"
#include "game/gameObject.h"

// initialize the lined list for abstractclassrep
AbstractClassRep *AbstractClassRep::sLast = nullptr;
std::unordered_map<std::string, AbstractClassRep*> AbstractClassRep::sClassRepMap;

ScriptEngine::ScriptEngine() {
	mEngine = nullptr;
}

ScriptEngine::~ScriptEngine() {
	delete mEngine;
}

bool ScriptEngine::init(const std::string &mainScript) {
	// only init once
	if (mEngine != nullptr) {
		assert(false);
		IO::printf("ScriptEngine::init() already initialized!");
		return false;
	}

	// init engine
	mEngine = new chaiscript::ChaiScript(chaiscript::Std_Lib::library());

	// glm::vec3 type
	{
		mEngine->add(chaiscript::user_type<glm::vec3>(), "vec3");
		mEngine->add(chaiscript::constructor<glm::vec3(const F32&, const F32&, const F32&)>(), "vec3");

		// Expose x, y, and z properties
		mEngine->add(chaiscript::fun(&glm::vec3::x), "x");
		mEngine->add(chaiscript::fun(&glm::vec3::y), "y");
		mEngine->add(chaiscript::fun(&glm::vec3::z), "z");


		// expose operator overloading to script
		mEngine->add(chaiscript::fun(&vec3_add_vec3), "+");
		mEngine->add(chaiscript::fun(&vec3_sub_vec3), "-");
		mEngine->add(chaiscript::fun(&vec3_mul_float), "*");
		mEngine->add(chaiscript::fun(&vec3_div_float), "/");
		mEngine->add(chaiscript::fun(&vec3_add_vec3_mutate), "+=");
		mEngine->add(chaiscript::fun(&vec3_sub_vec3_mutate), "-=");
		mEngine->add(chaiscript::fun(&vec3_mul_float_mutate), "*=");
		mEngine->add(chaiscript::fun(&vec3_div_float_mutate), "/=");

		// expose methods
		mEngine->add(chaiscript::fun(&vec3_to_string), "toString");
	}

	// glm::quat type
	{
		mEngine->add(chaiscript::user_type<glm::quat>(), "quat");
		mEngine->add(chaiscript::constructor<glm::quat(const F32&, const F32&, const F32&, const F32&)>(), "quat");

		// Expose x, y, z, and w properties
		mEngine->add(chaiscript::fun(&glm::quat::x), "x");
		mEngine->add(chaiscript::fun(&glm::quat::y), "y");
		mEngine->add(chaiscript::fun(&glm::quat::z), "z");
		mEngine->add(chaiscript::fun(&glm::quat::w), "w");

		// expose methods
//		mEngine->add(chaiscript::fun(&glm::quat::toString), "toString");
	}

	// Expose all script objects
	AbstractClassRep::initScriptAPI(this);

	// Engine functions
	{
		mEngine->add(chaiscript::fun(&echo<std::string>), "echo");
		mEngine->add(chaiscript::fun(&echo<float>), "echo");
		mEngine->add(chaiscript::fun(&echo<int>), "echo");
	}

	// execute main.chai
	execScript(mainScript);
	return true;
}

bool ScriptEngine::execScript(const std::string &scriptFile) {
	try {
		mEngine->eval_file(scriptFile);
	}
	catch (chaiscript::exception::eval_error &e) {
		IO::printf("ScriptEngine::exec() - %s\n", e.what());
		return false;
	}
	return true;
}
