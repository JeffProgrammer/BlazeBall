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

BindTemplateScriptFunction(echo, string, echo<std::string>);
BindTemplateScriptFunction(echo, F32, echo<F32>);
BindTemplateScriptFunction(echo, S32, echo<S32>);

#endif // _SCRIPTENGINE_SCRIPTFUNCTION_H_