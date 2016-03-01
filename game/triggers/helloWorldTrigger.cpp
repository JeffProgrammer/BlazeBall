//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "triggers/helloWorldTrigger.h"

IMPLEMENT_BEHAVIOR(HelloWorldTrigger);

HelloWorldTrigger::HelloWorldTrigger() {
	mHelloMessage = "Hello, World!";
	mGoodbyeMessage = "Goodbye!";
	mTrigger = nullptr;
}

void HelloWorldTrigger::start(ScriptObject *gameObject) {
	mTrigger = dynamic_cast<Trigger*>(gameObject);
	assert(mTrigger != nullptr);
}

void HelloWorldTrigger::onEnterTrigger() {
	IO::printf("%s\n", mHelloMessage.c_str());
}

void HelloWorldTrigger::onLeaveTrigger() {
	IO::printf("%s\n", mGoodbyeMessage.c_str());
}