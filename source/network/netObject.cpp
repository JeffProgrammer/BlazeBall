//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
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
	return true;
}

bool NetObject::write(CharStream &stream) const {
	return true;
}