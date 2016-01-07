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

	virtual bool read(CharStream &stream);
	virtual bool write(CharStream &stream) const;
};

#endif
