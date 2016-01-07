//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
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
	 * Reads a packet of type CharStream stream on the client when it is received
	 * from the server.
	 * @param stream The CharStream from the client.
	 * @return true if the stream was readable.
	 * @note this is potentially not secure as we are trusting the client. Be
	 *  extra caucious when manipulating this data and for the love of god,
	 *  SANITIZE, SANITIZE, SANITIZE.
	 */
	virtual bool readClientPacket(CharStream &stream);

	/**
	 * Reads a packet of type CharStream stream on the server when the client
	 * object wants to send data to the server.
	 * @param stream The CharStream from the server.
	 * @return true if the stream was readable.
	 */
	virtual bool readServerPacket(CharStream &stream);

	/**
	 * Writes a packet of type CharStream stream on the client and sends it off to
	 * the server for interpretation.
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
	 * @param stream The CharStream on the server.
	 * @return true if the stream was writable.
	 */
	virtual bool writeServerPacket(CharStream &stream) const;

	virtual bool read(CharStream &stream); /* This is initially receiving the ghost from the server. */
	virtual bool write(CharStream &stream) const; /* This is initally sending the ghost to the client. */
};

#endif
