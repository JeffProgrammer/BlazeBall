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

bool ScriptEngine::init() {
	// only init once
	if (mEngine != nullptr) {
		assert(false);
		IO::printf("ScriptEngine::init() already initialized!");
		return false;
	}

	// init engine
	mEngine = new chaiscript::ChaiScript(chaiscript::Std_Lib::library());

	// Vector3 type
	{
		mEngine->add(chaiscript::user_type<Vector3>(), "Vector3");
		mEngine->add(chaiscript::constructor<Vector3(const F32&, const F32&, const F32&)>(), "Vector3");

		// Expose x, y, and z properties
		mEngine->add(chaiscript::fun(&Vector3::x), "x");
		mEngine->add(chaiscript::fun(&Vector3::y), "y");
		mEngine->add(chaiscript::fun(&Vector3::z), "z");
		
		// expose operator overloading to script
		mEngine->add(chaiscript::fun(&Vector3::operator=), "=");
		mEngine->add(chaiscript::fun(&Vector3::operator+), "+");
		mEngine->add(chaiscript::fun(&Vector3::operator-), "-");
		mEngine->add(chaiscript::fun(&Vector3::operator*), "*");
		mEngine->add(chaiscript::fun(&Vector3::operator/), "/");
		mEngine->add(chaiscript::fun(&Vector3::operator+=), "+=");
		mEngine->add(chaiscript::fun(&Vector3::operator-=), "-=");
		mEngine->add(chaiscript::fun(&Vector3::operator*=), "*=");
		mEngine->add(chaiscript::fun(&Vector3::operator/=), "/=");

		// expose methods
		mEngine->add(chaiscript::fun(&Vector3::toString), "toString");
	}

	// Expose script object
	{
		mEngine->add(chaiscript::user_type<ScriptObject>(), "ScriptObject");
		mEngine->add(chaiscript::constructor<ScriptObject()>(), "ScriptObject");
		mEngine->add(chaiscript::fun(&ScriptObject::setName), "setName");
		mEngine->add(chaiscript::fun(&ScriptObject::getName), "getName");
	}

	// Expose GameObject to script
	{
		mEngine->add(chaiscript::user_type<GameObject>(), "GameObject");
		mEngine->add(chaiscript::constructor<GameObject()>(), "GameObject");
		mEngine->add(chaiscript::base_class<ScriptObject, GameObject>());

		mEngine->add(chaiscript::fun(&GameObject::getPosition), "getPosition");
		mEngine->add(chaiscript::fun(&GameObject::setPosition), "setPosition");
		mEngine->add(chaiscript::fun(&GameObject::getScale), "getScale");
		mEngine->add(chaiscript::fun(&GameObject::setScale), "setScale");
	}

	// Engine functions
	{
		mEngine->add(chaiscript::fun(&echo), "echo");
	}

	// execute main.chai
	execScript("main.chai");
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
