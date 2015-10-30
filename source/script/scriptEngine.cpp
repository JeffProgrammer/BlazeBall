//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//
// ref: http://stackoverflow.com/questions/30296953/export-c-class-to-duktape
//------------------------------------------------------------------------------

#include "script/scriptEngine.h"

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

	duk_push_c_function(mContext, js_Point_constructor, 2);

	duk_push_object(mContext);
	duk_put_function_list(mContext, -1, jsPointMethods);
	duk_put_prop_string(mContext, -2, "prototype");
	duk_put_global_string(mContext, "Point");
}

std::string ScriptEngine::eval(const char *str) {
	duk_peval_string(mContext, str);

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

duk_ret_t js_Point_destructor(duk_context *context) {
	// check if we have to delete the object
	duk_get_prop_string(context, 0, "___deleted");
	bool deleted = duk_to_boolean(context, -1);
	duk_pop(context);

	if (!deleted) {
		// we haven't deleted it yet, go delete
		duk_get_prop_string(context, 0, "___pointer");
		Point *p =  static_cast<Point*>(duk_to_pointer(context, -1));
		delete p;
		duk_pop(context);

		// set deleted flag
		duk_push_boolean(context, true);
		duk_put_prop_string(context, 0, "___deleted");

		printf("destroying point with address of: %x\n", p);
	}
	return 0;
}

duk_ret_t js_Point_constructor(duk_context *context) {
	F32 x = duk_require_number(context, 0);
	F32 y = duk_require_number(context, 1);

	// constructing...
	duk_push_this(context);

	// create point
	Point *p = new Point(x, y);

	// store a handle ID
	std::string handle = "___pointer";
	duk_push_pointer(context, p);
	duk_put_prop_string(context, -2, handle.c_str());

	// mark as we do not delete in case it's GC'd multiple times
	duk_push_boolean(context, false);
	duk_put_prop_string(context, -2, "___deleted");

	// store the destructor
	duk_push_c_function(context, js_Point_destructor, 1);
	duk_set_finalizer(context, -2);

	printf("constructing point with address of: %x\n", p);

	return 0;
}

duk_ret_t js_Point_getX(duk_context *context) {
	duk_push_this(context);
	duk_get_prop_string(context, -1, "___pointer");
	Point *point = static_cast<Point*>(duk_to_pointer(context, -1));
	duk_pop(context);
	duk_push_sprintf(context, "%f", point->mX);

	printf("c++: point.mX is: %f\n", point->mX);

	return 1;
}