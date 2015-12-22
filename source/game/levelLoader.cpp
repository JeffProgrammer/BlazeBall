//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "game/levelLoader.h"
#include "base/types.h"
#include "base/io.h"
#include "scriptEngine/abstractClassRep.h"
#include <rapidjson/document.h>

#ifdef __APPLE__
#define stricmp strcasecmp
#else
//#define stricmp stricmp
#endif

bool loadLevel(const std::string &file) {
	if (!IO::isfile(file)) {
		printf("Unable to load level file: %s\n", file.c_str());
		return false;
	}

	// read contents of the json file
	U32 fileLength;
	const char *contents = reinterpret_cast<const char*>(IO::readFile(file, fileLength));

	rapidjson::Document document;
	document.Parse(contents);

	for (auto obj = document.Begin(); obj != document.End(); ++obj) {
		// make sure this is an object. We can only have an array of objects.
		if (!obj->IsObject()) {
			printf("The level file %s can only have an arra of objects within the JSON structure\n", file.c_str());
			continue;
		}

		// get class name
		if (!obj->HasMember("class")) {
			printf("In %s, an object could not created as a class field was not specified.\n", file.c_str());
			continue;
		}
		const char *klass = (*obj)["class"].GetString();

		// create object here.
		ScriptObject *scriptObject = AbstractClassRep::createFromName(klass);

		// loop through each field
		for (auto field = obj->MemberBegin(); field != obj->MemberEnd(); ++field) {
			if (!field->name.IsString()) {
				printf("In %s, all object field names must declared in levels must be strings.\n", file.c_str());
				continue;
			}
			const char *fieldName = field->name.GetString();
			const char *fieldValue = field->value.GetString();

			// we allready got the class field.
			if (stricmp(fieldName, "class") == 0)
				continue;

			//Try and set the field
			if (!scriptObject->setField(fieldName, fieldValue)) {
				printf("Could not set class field %s on an object of type %s\n", fieldName, klass);
			}
		}
	}

	delete[] contents;
	return true;
}