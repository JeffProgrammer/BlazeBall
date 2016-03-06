//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//
// Note: portions of this code is from the assimp sample code:
// https://github.com/assimp/assimp/blob/master/samples/SimpleOpenGL/Sample_SimpleOpenGL.c
// https://github.com/assimp/assimp/tree/master/samples/SimpleAssimpViewX
//------------------------------------------------------------------------------

#include "resource/meshResource.h"
#include "resource/resourceLoader.h"

MeshResource::MeshResource(const std::string &file) {
	mResourceFile = file;
}

MeshResource::~MeshResource() {

}

bool MeshResource::load() {
	const aiScene *scene = aiImportFile(mResourceFile.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene == nullptr)
		return false;

	mAssimpScene.scene = scene;

	// get scene center and world box
	aiVector3D min, max;
	_getBoundingBox(scene, &min, &max);
	mAssimpScene.sceneMin.x = min.x;
	mAssimpScene.sceneMin.y = min.y;
	mAssimpScene.sceneMin.z = min.z;
	mAssimpScene.sceneMax.x = max.x;
	mAssimpScene.sceneMax.y = max.y;
	mAssimpScene.sceneMax.z = max.z;
	mAssimpScene.sceneCenter.x = (mAssimpScene.sceneMin.x + mAssimpScene.sceneMax.x) / 2.0f;
	mAssimpScene.sceneCenter.y = (mAssimpScene.sceneMin.y + mAssimpScene.sceneMax.y) / 2.0f;
	mAssimpScene.sceneCenter.z = (mAssimpScene.sceneMin.z + mAssimpScene.sceneMax.z) / 2.0f;

	// read the mesh using the assimp api calls.
	_readMesh();
	return true;
}

//-----------------------------------------------------------------------------
// Loading done with Assimp.
//-----------------------------------------------------------------------------

void MeshResource::_getBoundingBoxNode(const aiScene *scene, const aiNode *node, aiVector3D *min, aiVector3D *max, aiMatrix4x4 *transform) {
	aiMatrix4x4 prev = *transform;
	U32 i, j, numMeshes, numVerts;

	aiMultiplyMatrix4(transform, &node->mTransformation);

	numMeshes = node->mNumMeshes;
	for (i = 0; i < numMeshes; i++) {
		const aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		numVerts = mesh->mNumVertices;
		for (j = 0; j < numVerts; j++) {
			aiVector3D tmp = mesh->mVertices[j];
			aiTransformVecByMatrix4(&tmp, transform);

			min->x = min->x < tmp.x ? min->x : tmp.x;
			min->y = min->y < tmp.y ? min->y : tmp.y;
			min->z = min->z < tmp.z ? min->z : tmp.z;

			max->x = tmp.x > max->x ? tmp.x : max->x;
			max->y = tmp.y > max->y ? tmp.y : max->y;
			max->z = tmp.z > max->z ? tmp.z : max->z;
		}
	}

	// children
	numMeshes = node->mNumChildren;
	for (i = 0; i < numMeshes; i++)
		_getBoundingBoxNode(scene, node->mChildren[i], min, max, transform);
	*transform = prev;
}

void MeshResource::_getBoundingBox(const aiScene *scene, aiVector3D *min, aiVector3D *max) {
	aiMatrix4x4 transform;
	aiIdentityMatrix4(&transform);

	min->x = min->y = min->z = 1e10f;
	max->x = max->y = max->z = -1e10f;
	_getBoundingBoxNode(scene, scene->mRootNode, min, max, &transform);
}

bool MeshResource::_generateMaterial(const aiMesh *mesh, Material *&material) {
	aiString path;
	aiMaterial *aiMat = mAssimpScene.scene->mMaterials[mesh->mMaterialIndex];
	if (aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &path) != AI_SUCCESS) {
		// failed to load a texture
		IO::printf("Failed to load texture at path: %s\n", path.C_Str());
		material = nullptr;
		return false;
	}
	std::string directory = ResourceLoader::get()->getCurrentDirectory() + DIR_SEP + IO::getPath(std::string(path.C_Str()));
	std::string materialName = IO::getBase(std::string(path.C_Str()), '/');
	std::string diffuseFile = Texture::find(directory + DIR_SEP + materialName);

	//If we can't find it, just chuck the lot and keep going.
	if (!diffuseFile.length()) {
		IO::printf("Error in reading bitmap: %s Bitmap not found.\n", materialName.c_str());
		material = nullptr;
		return false;
	}
	std::string normalName = materialName + ".normal";
	std::string normalFile = Texture::find(directory + DIR_SEP + normalName);
	std::string specularName = materialName + ".alpha";
	std::string specularFile = Texture::find(directory + DIR_SEP + specularName);

	material = new Material(diffuseFile, normalFile, specularFile);
	material->setShader(Shader::getShaderByName("Model"));
	return true;
}