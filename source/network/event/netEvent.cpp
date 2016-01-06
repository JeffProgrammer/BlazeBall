//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "netEvent.h"

const U8 NetEvent::Magic = 0x24;

CharStream NetEvent::serialize() const {
	CharStream stream;
	if (write(stream)) {
		return stream;
	}
	return CharStream();
}

bool NetEvent::write(CharStream &data) const {
	//So we can identify as a NetEvent
	data.push<U8>(Magic);
	data.push<U32>(type);
	return true;
}

bool NetEvent::read(CharStream &data) {
	//Don't pop it off, because the subclasses need it
	return (data.peek<U8>() == Magic);
}