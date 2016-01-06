//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _NETWORK_EVENT_NETEVENT_H_
#define _NETWORK_EVENT_NETEVENT_H_

#include "base/types.h"
#include "base/event.h"

class NetEvent : public Event {
public:
	NetEvent(Type type) : Event(type) {}

	virtual U8 *serialize(U32 &length) = 0;
	virtual void deserialize(const U8 *data, const U32 &length) = 0;
};

#endif
