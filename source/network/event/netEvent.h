//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the project nor the
//      names of its contributors may be used to endorse or promote products
//      derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

#ifndef _NETWORK_EVENT_NETEVENT_H_
#define _NETWORK_EVENT_NETEVENT_H_

#include <unordered_map>
#include <memory>

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
	virtual CharStream serialize() const;

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
	virtual bool write(CharStream &data) const;
	/**
	 * Read the event's data from a CharStream, called from deserialize()
	 * @param data The stream from which the event's data is read
	 * @return If the read succeeded
	 */
	virtual bool read(CharStream &data);
};

#endif // _NETWORK_EVENT_NETEVENT_H_