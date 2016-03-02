//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "netObject.h"

bool NetObject::readClientPacket(CharStream &stream) {
	return true;
}

bool NetObject::readServerPacket(CharStream &stream) {
	return true;
}

bool NetObject::writeClientPacket(CharStream &stream) const {
	return true;
}

bool NetObject::writeServerPacket(CharStream &stream) const {
	return true;
}

bool NetObject::read(CharStream &stream) {
	mBehaviorString = stream.pop<std::string>();
	return true;
}

bool NetObject::write(CharStream &stream) const {
	stream.push<std::string>(mBehaviorString);
	return true;
}

void NetObject::initFields() {
	// Nothing.
}