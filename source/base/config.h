//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _BASE_CONFIG_H_
#define _BASE_CONFIG_H_

#include "types.h"
#include "io.h"
#include "platform/platformEvent.h"

class Config {
protected:
	std::map<std::string, int> mDefaultKeyMap;
	std::map<std::string, int> mKeyMap;

	void loadDefaults();
	void load(const std::string &path);
public:
	Config(const std::string &path);


	inline int getKey(const std::string &name) {
		if (mKeyMap.find(name) == mKeyMap.end())
			return getDefaultKey(name);
		return mKeyMap[name];
	}

	inline int getDefaultKey(const std::string &name) {
		return mDefaultKeyMap[name];
	}
};

#endif // _BASE_CONFIG_H_
