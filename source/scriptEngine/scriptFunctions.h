//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _SCRIPTENGINE_SCRIPTFUNCTION_H_
#define _SCRIPTENGINE_SCRIPTFUNCTION_H_

#include "scriptEngine/scriptEngine.h"

// script function
static void echo(std::string &in) {
	IO::printf("%s\n", in.c_str());
}

#endif // _SCRIPTENGINE_SCRIPTFUNCTION_H_