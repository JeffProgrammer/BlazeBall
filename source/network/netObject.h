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

#ifndef _NETWORK_NETOBJECT_H_
#define _NETWORK_NETOBJECT_H_

#include "base/types.h"
#include "game/scriptObject.h"
#include "network/charStream.h"

class NetObject : public ScriptObject {
public:
	NetObject() {

	}

	/**
	 * Reads a packet of type CharStream stream on the server when it is received
	 * from the client.
	 * Called on the Server. Transfer Client->Server.
	 * @param stream The CharStream from the client.
	 * @return true if the stream was readable.
	 * @note this is potentially not secure as we are trusting the client. Be
	 *  extra caucious when manipulating this data and for the love of god,
	 *  SANITIZE, SANITIZE, SANITIZE.
	 */
	virtual bool readClientPacket(CharStream &stream);

	/**
	 * Reads a packet of type CharStream stream on the client when the server
	 * sends data.
	 * Called on the Client. Transfer Server->Client.
	 * @param stream The CharStream from the server.
	 * @return true if the stream was readable.
	 */
	virtual bool readServerPacket(CharStream &stream);

	/**
	 * Writes a packet of type CharStream stream on the client and sends it off to
	 * the server for interpretation.
	 * Called on the Client. Transfer Client->Server.
	 * @param stream The CharStream on the client.
	 * @return true if the stream was writable.
	 * @note this is potentially not secure as we are trusting the client. Be
	 *  extra caucious when manipulating this data and for the love of god,
	 *  SANITIZE, SANITIZE, SANITIZE.
	 */
	virtual bool writeClientPacket(CharStream &stream) const;

	/**
	 * Writes a packet of type CharStream stream on the server and sends it off to
	 * the client for interpretation.
	 * Called on the Server. Transfer Server->Client.
	 * @param stream The CharStream on the server.
	 * @return true if the stream was writable.
	 */
	virtual bool writeServerPacket(CharStream &stream) const;

	virtual bool read(CharStream &stream); /* This is initially receiving the ghost from the server. */
	virtual bool write(CharStream &stream) const; /* This is initally sending the ghost to the client. */

	static void initFields();
};

#endif // _NETWORK_NETOBJECT_H_