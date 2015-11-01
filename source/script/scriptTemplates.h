//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//
// ref: http://stackoverflow.com/questions/30296953/export-c-class-to-duktape
//------------------------------------------------------------------------------

#ifndef _SCRIPT_SCRIPTTEMPLATES_H_
#define _SCRIPT_SCRIPTTEMPLATES_H_

#include "script/scriptUtil.h"

//------------------------------------------------------------------------------
// The ScriptReturnFunction Template
//------------------------------------------------------------------------------

//Generic return
template<typename T>
struct ScriptFunctionReturn {
	static const int code = 1;
	typedef T(*Method)(duk_context *, S32);
	static int perform(duk_context *context, Method method, S32 numArgs) {
		T var = method(context, numArgs);
		duk_pop(context);
		pushVar(context, var);
		return code;
	}

	static void pushVar(duk_context *context, T var);
};
//Void-only return, doesn't try to get the retval
template<>
struct ScriptFunctionReturn<void> {
	static const int code = 0;
	typedef void(*Method)(duk_context *, S32);
	static int perform(duk_context *context, Method method, S32 numArgs) {
		method(context, numArgs);
		duk_pop(context);
		return code;
	}
};

//Specialized pushVar() implementations
template<>
FORCE_SCRIPT_INLINE void ScriptFunctionReturn<S32>::pushVar(duk_context *context, S32 var) {
	duk_push_int(context, var);
}
template<>
FORCE_SCRIPT_INLINE void ScriptFunctionReturn<F32>::pushVar(duk_context *context, F32 var) {
	duk_push_number(context, var);
}
template<>
FORCE_SCRIPT_INLINE void ScriptFunctionReturn<bool>::pushVar(duk_context *context, bool var) {
	duk_push_boolean(context, var);
}
template<>
FORCE_SCRIPT_INLINE void ScriptFunctionReturn<const char *>::pushVar(duk_context *context, const char *var) {
	duk_push_string(context, var);
}

//------------------------------------------------------------------------------
// The ScriptMethodReturn Template.
//------------------------------------------------------------------------------

// Generic return
template<typename T>
struct ScriptMethodReturn {
	static const int code = 1;

	template <typename K>
	static int perform(duk_context *context, T(*method)(duk_context*, K*, S32), S32 numArgs) {
		duk_push_this(context);
		duk_get_prop_string(context, -1, "___pointer");
		K *object = static_cast<K *>(duk_to_pointer(context, -1));

		T var = method(context, object, numArgs);
		duk_pop(context);
		pushVar(context, var);
		return code;
	}

	static void pushVar(duk_context *context, T var);
};
// Void-only return, doesn't try to get the retval
template<>
struct ScriptMethodReturn<void> {
	static const int code = 0;

	template<typename K>
	static int perform(duk_context *context, void(*method)(duk_context*, K*, S32), S32 numArgs) {
		duk_push_this(context);
		duk_get_prop_string(context, -1, "___pointer");
		K *object = static_cast<K *>(duk_to_pointer(context, -1));

		method(context, object, numArgs);
		duk_pop(context);
		return code;
	}
};

//Specialized pushVar() implementations
template<>
FORCE_SCRIPT_INLINE void ScriptMethodReturn<S32>::pushVar(duk_context *context, S32 var) {
	duk_push_int(context, var);
}
template<>
FORCE_SCRIPT_INLINE void ScriptMethodReturn<F32>::pushVar(duk_context *context, F32 var) {
	duk_push_number(context, var);
}
template<>
FORCE_SCRIPT_INLINE void ScriptMethodReturn<bool>::pushVar(duk_context *context, bool var) {
	duk_push_boolean(context, var);
}
template<>
FORCE_SCRIPT_INLINE void ScriptMethodReturn<const char *>::pushVar(duk_context *context, const char *var) {
	duk_push_string(context, var);
}

//------------------------------------------------------------------------------
// The ScriptParam Template.
//------------------------------------------------------------------------------

struct ScriptParam {
	template <typename T>
	static T get(duk_context *context, U32 index);
};

template<> FORCE_SCRIPT_INLINE U32 ScriptParam::get(duk_context *context, U32 index) {
	return duk_require_int(context, index);
}
template<> FORCE_SCRIPT_INLINE F32 ScriptParam::get(duk_context *context, U32 index) {
	return static_cast<F32>(duk_require_number(context, index));
}
template<> FORCE_SCRIPT_INLINE F64 ScriptParam::get(duk_context *context, U32 index) {
	return duk_require_number(context, index);
}
template<> FORCE_SCRIPT_INLINE const char *ScriptParam::get(duk_context *context, U32 index) {
	return duk_require_string(context, index);
}

#endif // _SCRIPT_SCRIPTTEMPLATES_H_