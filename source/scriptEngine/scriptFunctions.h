//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _SCRIPTENGINE_SCRIPTFUNCTION_H_
#define _SCRIPTENGINE_SCRIPTFUNCTION_H_

#include <string>

#include "scriptEngine/scriptEngine.h"
#include "scriptEngine/scriptAPI.h"
#include "scriptEngine/abstractClassRep.h"
#include "game/scriptObject.h"

// script function
template<typename T>
static void echo(const T &in) {
	IO::printf("%s\n", std::to_string(in).c_str());
}

template<>
static void echo(const std::string &in) {
	IO::printf("%s\n", in.c_str());
}

void printAdd(S32 a, S32 b) {
	IO::printf("%d + %d = %d\n", a, b, a + b);
}

void printAdd(F64 a, F64 b) {
	IO::printf("%f + %f = %f\n", a, b, a + b);
}

void printAdd(F32 a, F32 b) {
	IO::printf("%f + %f = %f\n", a, b, a + b);
}

// Echo
BindTemplatedScriptFunction(echo, string, echo<std::string>);
BindTemplatedScriptFunction(echo, F32, echo<F32>);
BindTemplatedScriptFunction(echo, S32, echo<S32>);

// printAdd
BindOverloadedScriptFunction(printAdd, int_fn, void, (S32, S32), printAdd);
BindOverloadedScriptFunction(printAdd, float_fn, void, (F32, F32), printAdd);
BindOverloadedScriptFunction(printAdd, double_fn, void, (F64, F64), printAdd);

#endif // _SCRIPTENGINE_SCRIPTFUNCTION_H_