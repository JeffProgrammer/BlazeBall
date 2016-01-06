//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _NETWORK_EVENT_NETEVENT_H_
#define _NETWORK_EVENT_NETEVENT_H_

#include <unordered_map>

#include "base/types.h"
#include "base/event.h"
#include "network/charStream.h"

class NetEvent : public Event {
protected:
	/**
	 * Magic numbers so we know if we're actually a NetEvent
	 */
	static const U8 Magic;
public:
	NetEvent(Type type) : Event(type) {}

	/**
	 * Serialize the NetEvent to a CharStream, which can be sent over the network
	 * and used to deserialize the object later
	 * @return The CharStream containing the representation of the event
	 */
	CharStream serialize();

	/**
	 * Deserialize a CharStream into a NetEvent object that can be used.
	 * @param data The stream containing the event's data
	 * @return An event, if the deserialization succeeded.
	 */
	//This is defined in the client/server subclasses because they need
	// client/server to be passed as well.
	//static NetEvent *deserialize(CharStream &data);

protected:
	/**
	 * Write the event to a CharStream, called from serialize()
	 * @param data The stream to which the event's data is written
	 * @return If the write succeeded
	 */
	virtual bool write(CharStream &data);
	/**
	 * Read the event's data from a CharStream, called from deserialize()
	 * @param data The stream from which the event's data is read
	 * @return If the read succeeded
	 */
	virtual bool read(CharStream &data);
};

#endif
