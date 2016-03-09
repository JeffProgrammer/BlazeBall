//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include "resource/resourceLoader.h"
#include "resource/materialResource.h"
#include "resource/materialResourceFile.h"
#include "resource/interiorResource.h"
#include "resource/shapeResource.h"
#include "render/texture/bitmapTexture.h"

// For getcwd
#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd
#else
#include <unistd.h>
#endif

ResourceLoader *ResourceLoader::sResourceLoader = nullptr;

void ResourceLoader::create() {
	sResourceLoader = new ResourceLoader();
}

void ResourceLoader::destroy() {
	delete sResourceLoader;
}

ResourceLoader::ResourceLoader() {
	char buffer[1024]; // If your path is longer than this, idk what to say.
	mCurrentWorkingDir = getcwd(buffer, 1024);
}

ResourceLoader::~ResourceLoader() {

}

ShapeResource* ResourceLoader::loadShape(const std::string &file) {
	// See if it's already in the map. If it is, we are good.
	if (containsResource(file))
		return static_cast<ShapeResource*>(mMeshResourceMap[file]);

	// Make sure that this file exists!
	if (!IO::isfile(file)) {
		IO::printf("Unable to find mesh resource %s!\n", file.c_str());
		return nullptr;
	}

	auto resource = new ShapeResource(file);
	if (resource->load()) {
		// If we loaded the resource successfully, then store the resource
		// pointer to our hash maps and then return the resource.
		mResourceMap[file] = resource;
		mMeshResourceMap[file] = resource;
		return resource;
	}
	return nullptr;
}

InteriorResource* ResourceLoader::loadInterior(const std::string &file) {
	// See if it's already in the map. If it is, we are good.
	if (containsResource(file))
		return static_cast<InteriorResource*>(mMeshResourceMap[file]);

	// Make sure that this file exists!
	if (!IO::isfile(file)) {
		IO::printf("Unable to find interior resource %s!\n", file.c_str());
		return nullptr;
	}

	auto resource = new InteriorResource(file);
	if (resource->load()) {
		// If we loaded the resource successfully, then store the resource
		// pointer to our hash maps and then return the resource.
		mResourceMap[file] = resource;
		mMeshResourceMap[file] = resource;
		return resource;
	}
	return nullptr;
}

void ResourceLoader::loadMaterials(const std::string &file) {
	if (containsResource(file))
		return;

	if (!IO::isfile(file)) {
		IO::printf("Unable to find material json resource file %s!\n", file.c_str());
		return;
	}

	// read the json file
	U32 fileLength;
	const char *contents = reinterpret_cast<const char*>(IO::readFile(file, fileLength));
	rapidjson::Document document;
	document.Parse(contents);
	
	for (auto obj = document.Begin(); obj != document.End(); ++obj) {
		// make sure this is an object. We can only have an array of objects.
		if (!obj->IsObject()) {
			IO::printf("The level file %s can only have an array of objects within the JSON structure.\n", file.c_str());
			continue;
		}

		bool fail = false;
		std::string name;
		Shader *shader;
		std::vector<std::pair<Texture*, GLuint>> textures;

		// Loop through members.
		for (auto member = obj->MemberBegin(); member != obj->MemberEnd(); ++member) {
			std::string fieldName = member->name.GetString();
			if (fieldName == "name") {
				if (!member->value.IsString()) {
					IO::printf("Material Warning: Material name must be value type string! Setting to nil.\n");
					name = "";
				} else {
					name = member->value.GetString();
				}
			} else if (fieldName == "shader") {
				if (!member->value.IsString()) {
					IO::printf("Material Error: The shader value must be of type string!\n");
					fail = true;
					break;
				}
				
				shader = Shader::getShaderByName(member->value.GetString());
				if (shader == nullptr) {
					IO::printf("Material Error: Unable to find shader %s!\n", member->value.GetString());
					fail = true;
					break;
				}
			} else if (fieldName == "textures") {
				if (!member->value.IsObject()) {
					IO::printf("Material Error: The textures field must implement an object for its value!\n");
					fail = true;
					break;
				}

				// You know this code is messy when you have to call a variable doubleFailure.
				bool doubleFailure = false;
				for (auto innerMember = member->value.MemberBegin(); innerMember != member->value.MemberEnd(); ++innerMember) {
					// TODO: write a isNum string function to check this.
					U32 id = U32(atoi(innerMember->name.GetString()));

					if (!innerMember->value.IsString()) {
						IO::printf("MaterialError: Texture value must be of type string!\n");
						doubleFailure = true;
						break;
					}

					// Now check to make sure that the texture exists as a file.
					if (!IO::isfile(innerMember->value.GetString())) {
						IO::printf("Material Error: Texture %s is not a file!\n", innerMember->value.GetString());
						doubleFailure = true;
						break;
					}

					auto bitmap = IO::loadTexture(innerMember->value.GetString());
					textures.push_back(std::pair<Texture*, GLuint>(bitmap, id));
				}

				if (doubleFailure == true) {
					IO::printf("Material Error: The textures could not be configured from the JSON object!\n");
					fail = true;
					break;
				}
			} else {
				IO::printf("Material Warning: Unknown field %s\n");
			}
		}

		if (!fail) {
			if (textures.size() == 0) {
				IO::printf("Material Error: No textures have been found. Unable to create material %s", name.c_str());
				continue;
			}

			// Create the material resource.
			auto resource = new MaterialResource(name, textures);
			resource->setShader(shader);
			mMaterialResources.push_back(resource);
		} else {
			IO::printf("MaterialError: Unable to create material %s\n", name.c_str());
		}
	}

	// cache this file as a resource.
	auto resource = new MaterialResourceFile(file);
	mResourceMap[file] = resource;
}