//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//
// ref: http://stackoverflow.com/questions/30296953/export-c-class-to-duktape
//------------------------------------------------------------------------------

#ifndef _SCRIPT_SCRIPTUTIL_H_
#define _SCRIPT_SCRIPTUTIL_H_

#include <duktape.h>
#include <string>
#include "base/types.h"

#ifdef _MSC_VER
#define FORCE_SCRIPT_INLINE __forceinline
#elif defined(__llvm__) || defined(__GNUC__)
#define FORCE_SCRIPT_INLINE inline __attribute__((always_inline))
#else
#define FORCE_SCRIPT_INLINE inline
#endif

/**
 * Checks the constructor to make sure that the required number of parameters
 * where met, as well as making sure that the constructor was called with the
 * new keyword from javascript.
 * @param context The heap context of the javascript virtual machine.
 * @param className The name of the class this contructor belongs to.
 * @param numArgs The number of arguments for the class's constructor.
 * @return 0 if success, DUK_RET_API_ERROR if new wasn't called, or 
 *  DUK_RET_RANGE_ERROR if the number of arguments didn't match.
 * @note FOR INTERNAL MACRO USE ONLY.
 */
FORCE_SCRIPT_INLINE duk_ret_t __checkConstructor(duk_context *context, const char *className, S32 numArgs) {
	if (!duk_is_constructor_call(context)) {
		printf("You must call the constructor with new.\n"); 
		return DUK_RET_API_ERROR; 
	}

	// check for num args to make sure its right
	if (duk_get_top(context) != numArgs) {
		printf("Call to constructor of class %s takes %i arguments. You passed %i arguments.\n", className, duk_get_top(context), numArgs);
		return DUK_RET_RANGE_ERROR;
	} 
	return 0;
}

/**
 * Finishes a constructor by doing some bookeeping work for the javascript-c++
 * bridge.
 * @param context The heap context of the javascript virtual machine.
 * @param function the c++ destructor function to be called whenever the garbage
 *  collector deletes this object.
 * @param instance The c++ pointer that the javascript object represents.
 * @note FOR INTERNAL MACRO USE ONLY.
 */
FORCE_SCRIPT_INLINE void __finishConstructor(duk_context *context, duk_c_function function, void *instance) {
	duk_push_this(context);
	// Store handle ID 
	std::string handle = "___pointer";
	duk_push_pointer(context, instance);
	duk_put_prop_string(context, -2, handle.c_str());

	/* store destructor */
	duk_push_c_function(context, function, 1);
	duk_set_finalizer(context, -2);
}

#endif // _SCRIPT_SCRIPTUTIL_H_