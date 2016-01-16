//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _SCRIPTENGINE_SCRIPTFUNCTION_H_
#define _SCRIPTENGINE_SCRIPTFUNCTION_H_

#include <string>

#include "scriptEngine/scriptEngine.h"
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

#endif // _SCRIPTENGINE_SCRIPTFUNCTION_H_