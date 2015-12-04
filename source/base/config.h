//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef config_h
#define config_h

#include "types.h"
#include "io.h"
#include "platform/event.h"

class Config {
protected:
	std::map<std::string, KeyEvent::Key> mDefaultKeyMap;
	std::map<std::string, KeyEvent::Key> mKeyMap;

	void loadDefaults();
	void load(const std::string &path);
public:
	Config(const std::string &path);


	inline KeyEvent::Key getKey(const std::string &name) {
		if (mKeyMap.find(name) == mKeyMap.end())
			return getDefaultKey(name);
		return mKeyMap[name];
	}

	inline KeyEvent::Key getDefaultKey(const std::string &name) {
		return mDefaultKeyMap[name];
	}
};

#endif
