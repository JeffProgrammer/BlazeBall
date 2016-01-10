//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _SCRIPTENGINE_SCRIPTFUNCTION_H_
#define _SCRIPTENGINE_SCRIPTFUNCTION_H_

#include "scriptEngine/scriptEngine.h"
#include "scriptEngine/abstractClassRep.h"
#include "game/scriptObject.h"

// script function
static void echo(const std::string &in) {
	IO::printf("%s\n", in.c_str());
}

// don't go out of scope
std::vector<ScriptObject*> gFudgeCollector;

// create objects
static ScriptObject* createObject(std::string &klass) {
	auto obj = AbstractClassRep::createFromName(nullptr, klass);
	gFudgeCollector.push_back(obj);
	return obj;
}

#endif // _SCRIPTENGINE_SCRIPTFUNCTION_H_