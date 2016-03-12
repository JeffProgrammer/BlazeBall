//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include <unordered_map>
#include "resource/interiorResource.h"

struct InteriorInfo {
	std::unordered_map<U16, std::vector<Vertex>> map;
	std::vector<Material*> materialList;
};

InteriorResource::InteriorResource(const std::string &file) : MeshResource(file) {
	
}

InteriorResource::~InteriorResource() {

}

static InteriorInfo loadInterior(const DIF::DIF &dif, const std::string &dir) {
	InteriorInfo info;

	const DIF::Interior &interior = dif.interior[0];

	// Create an instance in the map for each material
	for (U32 i = 0; i < interior.surface.size(); i++) {
		const DIF::Interior::Surface &surface = interior.surface[i];
		glm::vec3 normal = interior.normal[interior.plane[surface.planeIndex].normalIndex];
		if (surface.planeFlipped)
			normal *= -1.0f;

		//New and improved rendering with actual Triangle Strips this time
		for (U32 j = surface.windingStart + 2; j < surface.windingStart + surface.windingCount; j++) {
			glm::vec3 v0, v1, v2;

			if ((j - (surface.windingStart + 2)) % 2 == 0) {
				v0 = interior.point[interior.index[j]];
				v1 = interior.point[interior.index[j - 1]];
				v2 = interior.point[interior.index[j - 2]];
			}
			else {
				v0 = interior.point[interior.index[j - 2]];
				v1 = interior.point[interior.index[j - 1]];
				v2 = interior.point[interior.index[j]];
			}

			DIF::Interior::TexGenEq texGenEq = interior.texGenEq[surface.texGenIndex];

			glm::vec2 uv0 = glm::vec2(planeF_distance_to_point(texGenEq.planeX, v0), planeF_distance_to_point(texGenEq.planeY, v0));
			glm::vec2 uv1 = glm::vec2(planeF_distance_to_point(texGenEq.planeX, v1), planeF_distance_to_point(texGenEq.planeY, v1));
			glm::vec2 uv2 = glm::vec2(planeF_distance_to_point(texGenEq.planeX, v2), planeF_distance_to_point(texGenEq.planeY, v2));

			glm::vec3 deltaPos1 = v1 - v0;
			glm::vec3 deltaPos2 = v2 - v0;
			glm::vec2 deltaUV1 = uv1 - uv0;
			glm::vec2 deltaUV2 = uv2 - uv0;

			F32 r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

			glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
			glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

			tangent = glm::normalize(tangent - (normal * glm::dot(normal, tangent)));
			if (glm::dot(glm::cross(normal, tangent), bitangent) < 0.0f) {
				tangent *= -1.0f;
			}

			tangent = glm::normalize(tangent);
			bitangent = glm::normalize(bitangent);

			Vertex pointA, pointB, pointC;
			pointA.point = v0; pointB.point = v1; pointC.point = v2;
			pointA.uv = uv0; pointB.uv = uv1; pointC.uv = uv2;
			pointA.normal = normal; pointB.normal = normal; pointC.normal = normal;
			pointA.tangent = tangent; pointB.tangent = tangent;  pointC.tangent = tangent;
			pointA.bitangent = bitangent; pointB.bitangent = bitangent; pointC.bitangent = bitangent;

			info.map[surface.textureIndex].push_back(pointA);
			info.map[surface.textureIndex].push_back(pointB);
			info.map[surface.textureIndex].push_back(pointC);
		}
	}

	// TODO: use material resource!!!
	//Allocate all textures for the interior
	if (interior.materialName.size()) {
		for (U32 i = 0; i < interior.materialName.size(); i++) {
			std::string materialName = IO::getName(interior.materialName[i]);
			std::string diffuseFile = Texture::find(dir + DIR_SEP + materialName);

			//If we can't find it, just chuck the lot and keep going.
			if (!diffuseFile.length()) {
				fprintf(stderr, "Error in reading bitmap: %s Bitmap not found.\n", materialName.c_str());
				assert(false);
				continue;
			}
			std::string normalName = materialName + ".normal";
			std::string normalFile = Texture::find(dir + DIR_SEP + normalName);
			std::string specularName = materialName + ".alpha";
			std::string specularFile = Texture::find(dir + DIR_SEP + specularName);

			Material *material = new Material(diffuseFile, normalFile, specularFile);

			//TODO: Make this material-dependant when we get around to material properties
			material->setShader(Shader::getShaderByName("Interior"));

			//Assign the texture
			info.materialList.push_back(material);
		}
	}

	return info;
}

bool InteriorResource::load() {
	DIF::DIF dif;
	std::string dir = IO::getPath(mResourceFile);
	std::ifstream file(mResourceFile, std::ios::binary);
	if (!dif.read(file)) {
		IO::printf("Unable to read the dif file %s!\n", mResourceFile.c_str());
		return false;
	}

	// Temporary cache, will be split into submeshes once interpreted from the data file.
	InteriorInfo &info = loadInterior(dif, dir);
	// Now create each submesh
	for (U32 i = 0; i < info.materialList.size(); i++) {
		SubMeshData data;
		data.material = info.materialList[i];
		data.primitive = GL_TRIANGLES;
		data.vertices = info.map[i];
		mSubMeshes.push_back(data);
	}

	return true;
}