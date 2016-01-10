//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "scriptEngine/scriptEngine.h"
#include "scriptEngine/abstractClassRep.h"
#include "scriptEngine/scriptFunctions.h"
#include "game/gameObject.h"

class Vector3 : public glm::vec3 {
public:
	Vector3() : glm::vec3() {

	}

	Vector3(const F32 &x, const F32 &y, const F32 &z) : glm::vec3(x, y, z) {}

	Vector3& operator=(const Vector3 &vec) {
		x = vec.x;
		y = vec.y;
		z = vec.z;
		return *this;
	}

	Vector3& operator+(const Vector3 &vec) {
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}

	Vector3& operator-(const Vector3 &vec) {
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		return *this;
	}

	Vector3& operator*(const F32 &scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	Vector3& operator/(const F32 &scalar) {
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}

	void operator+=(const Vector3 &vec) {
		x += vec.x;
		y += vec.y;
		z += vec.z;
	}

	void operator-=(const Vector3 &vec) {
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
	}

	void operator*=(const F32 &scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
	}

	void operator/=(const F32 &scalar) {
		x /= scalar;
		y /= scalar;
		z /= scalar;
	}

	std::string toString() const {
		return "[" + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + "]";
	}
};

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
		mEngine->add(chaiscript::fun(&ScriptObject::mName), "name");
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
	mEngine->eval_file(scriptFile);
	return true;
}
