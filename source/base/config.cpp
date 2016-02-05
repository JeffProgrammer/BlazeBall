//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "config.h"
#include <fstream>
#include <sstream>

Config::Config(const std::string &path) {
	loadDefaults();
	load(path);
}

void Config::loadDefaults() {
	mDefaultKeyMap["forward"]  = KeyEvent::KEY_W;
	mDefaultKeyMap["backward"] = KeyEvent::KEY_S;
	mDefaultKeyMap["left"]     = KeyEvent::KEY_A;
	mDefaultKeyMap["right"]    = KeyEvent::KEY_D;
}

void Config::load(const std::string &path) {
	//Read the file
	std::ifstream file(path);

	std::string line;
	while (std::getline(file, line, '\n')) {
		std::stringstream ss(line);

		std::string name, key;
		if (std::getline(ss, name, ';') && std::getline(ss, key, ';')) {
			mKeyMap[name] = static_cast<KeyEvent::Key>(key[0]);
		}
	}
}
