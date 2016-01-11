//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "scriptEngine/scriptEngine.h"

#include <chaiscript/chaiscript.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>

#include "scriptEngine/abstractClassRep.h"
#include "scriptEngine/scriptFunctions.h"
#include "game/gameObject.h"
#include "base/math/vector3.h"

// initialize the lined list for abstractclassrep
AbstractClassRep *AbstractClassRep::sLast = nullptr;
std::unordered_map<std::string, AbstractClassRep*> AbstractClassRep::sClassRepMap;

// I hate global variables, but how else can we just randomly access the script
// engine at any time. Also...server is seperate process from client. We need
// to have separate engines running.
// Make 2 script engines. One for client and one for server
ScriptEngine *gClientScriptEngine = nullptr;
ScriptEngine *gServerScriptEngine = nullptr;

ScriptEngine* ScriptEngine::get(Engine isServer) {
	if (isServer == SERVER) {
		if (gServerScriptEngine == nullptr)
			gServerScriptEngine = new ScriptEngine();
		return gServerScriptEngine;
	} else {
		if (gClientScriptEngine == nullptr)
			gClientScriptEngine = new ScriptEngine();
		return gClientScriptEngine;
	}
}

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
		mEngine->add(chaiscript::fun(&GameObject::getRotation), "getRotation");
		mEngine->add(chaiscript::fun(&GameObject::setRotation), "setRotation");
	}

	// Engine functions
	{
		mEngine->add(chaiscript::fun(&echo), "echo");
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
