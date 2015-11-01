//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//
// ref: http://stackoverflow.com/questions/30296953/export-c-class-to-duktape
//------------------------------------------------------------------------------

#include "script/scriptEngine.h"

std::vector<ScriptClassConstructor*> ScriptClassConstructor::sScriptConstructorVector;

IMPLEMENT_SCRIPT_OBJECT(Point, 2);

ScriptMethod(Point, setX, void, 1) {
	F32 x = static_cast<F32>(ScriptNumber(0));
	object->mX = x;
}

ScriptMethod(Point, getX, float, 0) {
	ScriptReturnNumber(object->mX);
}

ScriptMethod(Point, setY, void, 1) {
	F32 y = static_cast<F32>(ScriptNumber(0));
	object->mY = y;
}

ScriptMethod(Point, getY, float, 0) {
	ScriptReturnNumber(object->mY);
}

IMPLEMENT_SCRIPT_OBJECT(Point3, 3);
IMPLEMENT_SCRIPT_PARENT(Point3, Point);

ScriptMethod(Point3, setZ, void, 1) {
	F32 z = static_cast<F32>(ScriptNumber(0));
	object->mZ = z;
}

ScriptMethod(Point3, getZ, float, 0) {
	ScriptReturnNumber(object->mZ);
}

ScriptEngine::ScriptEngine() {
	mContext = nullptr;
}

ScriptEngine::~ScriptEngine() {
	if (mContext != nullptr) {
		duk_destroy_heap(mContext);
		mContext = nullptr;
	}
}

void ScriptEngine::init() {
	// initializes duk
	mContext = duk_create_heap_default();

	for (auto constructor : ScriptClassConstructor::sScriptConstructorVector) {
		
		// Push constructor
		duk_push_c_function(mContext, constructor->getConstructor().mConstructor, constructor->getConstructor().mNumArgs);
		duk_push_object(mContext);

		// add the parent prototype if one exists
		if (constructor->getParentName() != "") {
			bool x = duk_get_global_string(mContext, constructor->getParentName().c_str());
			duk_get_prop_string(mContext, -1, "prototype");
			duk_remove(mContext, -2);
			duk_set_prototype(mContext, -2);
		}

		// push each method
		const auto &methodList = constructor->getMethods();
		for (const auto &method : methodList) {
			// push method
			duk_push_c_function(mContext, method.mFunction, method.mNumArgs);
			duk_put_prop_string(mContext, -2, method.mName.c_str());
		}

		// push each field
		const auto &fieldList = constructor->getFields();
		for (const auto &field : fieldList) {
			duk_push_string(mContext, field.mName.c_str());
			duk_push_c_function(mContext, field.mGetterFn, 0);
			duk_push_c_function(mContext, field.mSetterFn, 1);
			duk_def_prop(mContext, -4, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER);
		}

		// set prototype
		duk_put_prop_string(mContext, -2, "prototype");
		duk_put_global_string(mContext, constructor->getName().c_str());
	}
}

std::string ScriptEngine::eval(const std::string &str) {
	if (duk_peval_string(mContext, str.c_str()) != 0) {
		printf("Error evaluating script: %s\n", duk_safe_to_string(mContext, -1));
		duk_pop(mContext);
		return "Syntax Error";
	}

	std::string ret = "";
	if (duk_is_string(mContext, -1)) {
		ret = duk_get_string(mContext, -1);
	} else if (duk_is_number(mContext, -1)) {
		F32 x = static_cast<F32>(duk_get_number(mContext, -1));
		duk_pop(mContext);
		return std::to_string(x);
	}
	duk_pop(mContext);
	return ret;
}

std::string ScriptEngine::exec(const std::string &file) {
	std::string result = "";

	if (duk_peval_file(mContext, file.c_str()) != 0) {
		printf("file %s failed to execute!\n", file.c_str());
		printf("Error: %s\n", duk_safe_to_string(mContext, -1));
		result = "<Syntax Error>";
	} else {
		// get result if one exists. If there is no result, return
		// empty string
		bool set = !duk_is_undefined(mContext, -1);
		std::string test = duk_safe_to_string(mContext, -1);
		if (set)
			result = test;
	}
	duk_pop(mContext);
	return result;
}