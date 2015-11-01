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

	js_Point_initialize(mContext);
	js_Point3_initialize(mContext);
}

void js_Point_initialize(duk_context *context) {
	/* Point.constructor */ duk_push_c_function(context, js_Point_constructor, 2); // stack: [ function ]
	/* Point.prototype */ duk_push_object(context); // stack: [ function, object ]

	// make getX() accessible from script
	/* Point.prototype.getX */ duk_push_c_function(context, js_Point_getX, 0); // stack: [ function, object, function ]
	duk_put_prop_string(context, -2, "getX"); // stack: [ function, object [ getX : function] ]

	//Make .x accessible, need [ key, getter, setter] on the top of stack
	/* Point.x */ duk_push_string(context, "x"); // stack: [ function, object [ getX : function ], string ]
	/* Point.x.getter */ duk_push_c_function(context, js_Point_getX, 0); // stack: [ function, object [ getX : function ], string, function ]
	/* Point.x.setter */ duk_push_c_function(context, js_Point_setX, 1); // stack: [ function, object [ getX : function ], string, function, function ]
	duk_def_prop(context, -4, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER); // stack: [ function, object [ getX : function, x : native ] ]

	// make getY() accessible from script
	/* Point.prototype.getY */ duk_push_c_function(context, js_Point_getY, 0); // stack: [ function, object, function ]
	duk_put_prop_string(context, -2, "getY"); // stack: [ function, object [ getX : function, getY : function ] ]

	/* Point.y */ duk_push_string(context, "y"); // stack: [ ..., object: Point, string: "y" ]
	/* Point.y.getter */ duk_push_c_function(context, js_Point_getY, 0); // stack: [ ..., object: Point, string: "y", function: "getY" ]
	/* Point.y.setter */ duk_push_c_function(context, js_Point_setY, 1); // stack: [ ..., object: Point, string: "y", function: "getY", function: "setY" ] 
	duk_def_prop(context, -4, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER); // stack: [ ..., object: Point ]

	duk_put_prop_string(context, -2, "prototype"); // stack: [ function [ prototype : object [ ... ] ] ]
	duk_put_global_string(context, "Point"); // stack: [ ], global: [ Point : function [ prototype : object [ ... ] ] ]
}

void js_Point3_initialize(duk_context *context) {
	/* Point3.constructor */ duk_push_c_function(context, js_Point3_constructor, 3); // stack: [ function ]
	/* Point3.prototype */ duk_push_object(context); // stack: [ function, object ]

	//Get Point.prototype so we can attach it
	/* Point */ duk_get_global_string(context, "Point"); // stack: [ function, object, Point ]
	/* Point.prototype */ duk_get_prop_string(context, -1, "prototype"); // stack: [ function, object, Point, Point.prototype ]
	duk_remove(context, -2); // stack: [ function, object, Point.prototype ]

	//Attach Point.prototype as the prototype for Point3's prototype. Point.prototype needs to be on the top of the stack
	duk_set_prototype(context, -2); // stack: [ function, object [ __proto__: Point.prototype ] ]

	//Attach getZ
	/* Point3.prototype.getZ */ duk_push_c_function(context, js_Point3_getZ, 0); // stack: [ function, object [ prototype: Point.prototype ], function ]
	duk_put_prop_string(context, -2, "getZ"); // stack: [ function, object [ prototype: Point.prototype, getZ : function ] ]

	duk_put_prop_string(context, -2, "prototype"); // stack: [ function [ prototype : object [ prototype: Point.prototype, getZ : function ] ] ]
	duk_put_global_string(context, "Point3"); // stack: [ ], global: [ Point3 : function [ prototype : object [ prototype: Point.prototype, getZ : function ] ] ]
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

duk_ret_t js_Point_constructor(duk_context *context) {
	// ensure we called the function with new to create
	// an object.
	if (!duk_is_constructor_call(context)) {
		printf("must call Point() with new\n");
		return DUK_RET_TYPE_ERROR;
	}

	S32 num = duk_get_top(context);

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

	printf("constructing point with address of: %p\n", p);

	return 0;
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

		printf("destroying point with address of: %p\n", p);
	}
	return 0;
}

duk_ret_t js_Point_getX(duk_context *context) {
	duk_push_this(context);
	duk_get_prop_string(context, -1, "___pointer");
	Point *point = static_cast<Point*>(duk_to_pointer(context, -1));
	duk_pop(context);
	duk_push_number(context, point->mX);

	printf("c++: point.mX is: %f\n", point->mX);

	return 1;
}

duk_ret_t js_Point_setX(duk_context *context) {
	duk_push_this(context);
	duk_get_prop_string(context, -1, "___pointer");
	Point *point = static_cast<Point*>(duk_to_pointer(context, -1));

	F32 x = duk_require_number(context, 0);
	point->mX = x;

	duk_pop(context);
	return 0; //Undefined
}

duk_ret_t js_Point_getY(duk_context *context) {
	duk_push_this(context);
	duk_get_prop_string(context, -1, "___pointer");
	Point *point = static_cast<Point*>(duk_to_pointer(context, -1));
	duk_pop(context);
	duk_push_number(context, point->mY);

	printf("c++: point.mY is: %f\n", point->mY);

	return 1;
}

duk_ret_t js_Point_setY(duk_context *context) {
	duk_push_this(context);
	duk_get_prop_string(context, -1, "___pointer");
	Point *point = static_cast<Point*>(duk_to_pointer(context, -1));

	F32 y = duk_require_number(context, 0);
	point->mY = y;

	duk_pop(context);
	return 0; //Undefined
}

duk_ret_t js_Point3_constructor(duk_context *context) {
	// ensure we called the function with new to create
	// an object.
	if (!duk_is_constructor_call(context)) {
		printf("must call Point3() with new\n");
		return DUK_RET_TYPE_ERROR;
	}

	F32 x = duk_require_number(context, 0);
	F32 y = duk_require_number(context, 1);
	F32 z = duk_require_number(context, 2);

	// constructing...
	duk_push_this(context);

	// create point
	Point3 *p = new Point3(x, y, z);

	// store a handle ID
	std::string handle = "___pointer";
	duk_push_pointer(context, p);
	duk_put_prop_string(context, -2, handle.c_str());

	// mark as we do not delete in case it's GC'd multiple times
	duk_push_boolean(context, false);
	duk_put_prop_string(context, -2, "___deleted");

	// store the destructor
	duk_push_c_function(context, js_Point3_destructor, 1);
	duk_set_finalizer(context, -2);

	printf("constructing point with address of: %p\n", p);

	return 0;
}

duk_ret_t js_Point3_destructor(duk_context *context) {
	// check if we have to delete the object
	duk_get_prop_string(context, 0, "___deleted");
	bool deleted = duk_to_boolean(context, -1);
	duk_pop(context);

	if (!deleted) {
		// we haven't deleted it yet, go delete
		duk_get_prop_string(context, 0, "___pointer");
		Point3 *p =  static_cast<Point3*>(duk_to_pointer(context, -1));
		delete p;
		duk_pop(context);

		// set deleted flag
		duk_push_boolean(context, true);
		duk_put_prop_string(context, 0, "___deleted");

		printf("destroying point with address of: %p\n", p);
	}
	return 0;
}

duk_ret_t js_Point3_getZ(duk_context *context) {
	duk_push_this(context);
	duk_get_prop_string(context, -1, "___pointer");
	Point3 *point = static_cast<Point3*>(duk_to_pointer(context, -1));
	duk_pop(context);
	duk_push_number(context, point->mZ);

	printf("c++: point3.mZ is: %f\n", point->mZ);

	return 1;
}

