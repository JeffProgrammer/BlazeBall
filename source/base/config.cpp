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
	//Read the file
	std::ifstream file("defaultConfig.txt");

	std::string line;
	while (std::getline(file, line, '\n')) {
		std::stringstream ss(line);

		std::string name, key;
		if (std::getline(ss, name, ';') && std::getline(ss, key, ';')) {
			mKeyMap[name] = std::atoi(key.c_str());
		}
	}
}

void Config::load(const std::string &path) {
	//Read the file
	std::ifstream file(path);

	std::string line;
	while (std::getline(file, line, '\n')) {
		std::stringstream ss(line);

		std::string name, key;
		if (std::getline(ss, name, ';') && std::getline(ss, key, ';')) {
			mKeyMap[name] = std::atoi(key.c_str());
		}
	}
}
