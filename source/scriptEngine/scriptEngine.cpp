//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "scriptEngine/scriptEngine.h"

#include "scriptEngine/abstractClassRep.h"
#include "scriptEngine/scriptFunctions.h"
#include "game/gameObject.h"
#include "base/math/vector3.h"

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

	// Vec3 type
	{
		mEngine->add(chaiscript::user_type<Vec3>(), "Vec3");
		mEngine->add(chaiscript::constructor<Vec3(const F32&, const F32&, const F32&)>(), "Vec3");

		// Expose x, y, and z properties
		mEngine->add(chaiscript::fun(&Vec3::x), "x");
		mEngine->add(chaiscript::fun(&Vec3::y), "y");
		mEngine->add(chaiscript::fun(&Vec3::z), "z");
		
		// expose operator overloading to script
		mEngine->add(chaiscript::fun(&Vec3::operator+), "+");
		mEngine->add(chaiscript::fun(&Vec3::operator-), "-");
		mEngine->add(chaiscript::fun(&Vec3::operator*), "*");
		mEngine->add(chaiscript::fun(&Vec3::operator/), "/");
		mEngine->add(chaiscript::fun(&Vec3::operator+=), "+=");
		mEngine->add(chaiscript::fun(&Vec3::operator-=), "-=");
		mEngine->add(chaiscript::fun(&Vec3::operator*=), "*=");
		mEngine->add(chaiscript::fun(&Vec3::operator/=), "/=");

		// expose methods
		mEngine->add(chaiscript::fun(&Vec3::toString), "toString");
	}

	// Quat type
	{
		mEngine->add(chaiscript::user_type<Quat>(), "Quat");
		mEngine->add(chaiscript::constructor<Quat(const F32&, const F32&, const F32&, const F32&)>(), "Quat");

		// Expose x, y, z, and w properties
		mEngine->add(chaiscript::fun(&Quat::x), "x");
		mEngine->add(chaiscript::fun(&Quat::y), "y");
		mEngine->add(chaiscript::fun(&Quat::z), "z");
		mEngine->add(chaiscript::fun(&Quat::w), "w");

		// expose methods
		mEngine->add(chaiscript::fun(&Quat::toString), "toString");
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
