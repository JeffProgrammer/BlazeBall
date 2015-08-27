//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of the project nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Note: portions of this code is from the assimp sample code:
// https://github.com/assimp/assimp/blob/master/samples/SimpleOpenGL/Sample_SimpleOpenGL.c
// https://github.com/assimp/assimp/tree/master/samples/SimpleAssimpViewX
//------------------------------------------------------------------------------

#include "render/modelManager.h"
#include "base/io.h"

// For getcwd
#ifdef _WIN32
#include <direct.h>

inline char* getcwd(char *buf, size_t size) {
	return _getcwd(buf, size);
}

#else
#include <unistd.h>
#endif

ModelManager::ModelManager() {
	char buffer[512]; // If your path is longer than this, then FUCK YOU
	mCurrentWorkingDir = getcwd(buffer, 512);
}

bool ModelManager::loadAsset(const std::string &file) {
	const aiScene *scene = aiImportFile(file.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene == nullptr)
		return false;

	ModelScene *modelScene = new ModelScene();
	modelScene->scene = scene;
	mResourceCache[file] = modelScene;

	// get scene center:
	_getBoundingBox(scene, &modelScene->sceneMin, &modelScene->sceneMax);
	modelScene->sceneCenter.x = (modelScene->sceneMin.x + modelScene->sceneMax.x) / 2.0f;
	modelScene->sceneCenter.y = (modelScene->sceneMin.y + modelScene->sceneMax.y) / 2.0f;
	modelScene->sceneCenter.z = (modelScene->sceneMin.z + modelScene->sceneMax.z) / 2.0f;

	_glCreateMesh(modelScene);

	return true;
}

bool ModelManager::releaseAsset(const std::string &file) {
	if (!containsModel(file))
		return false;

	// delete geometry on the GPU
	for (size_t i = 0; i < mResourceCache[file]->meshes.size(); i++) {
		const ModelScene::ModelMesh &mesh = mResourceCache[file]->meshes[i];
		glDeleteBuffers(1, &mesh.vbo);
		glDeleteBuffers(1, &mesh.ibo);
	}

	delete mResourceCache[file];
	mResourceCache[file] = nullptr;
	mResourceCache.erase(file);
	return true;
}

bool ModelManager::containsModel(const std::string &file) const {
	return mResourceCache.find(file) != mResourceCache.end();
}

void ModelManager::render() {

}

//------------------------------------------------------------------------------
// private support methods
//------------------------------------------------------------------------------

void ModelManager::_getBoundingBoxNode(const aiScene *scene, const aiNode *node, aiVector3D *min, aiVector3D *max, aiMatrix4x4 *transform) {
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

void ModelManager::_getBoundingBox(const aiScene *scene, aiVector3D *min, aiVector3D *max) {
	aiMatrix4x4 transform;
	aiIdentityMatrix4(&transform);

	min->x = min->y = min->z = 1e10f;
	max->x = max->y = max->z = -1e10f;
	_getBoundingBoxNode(scene, scene->mRootNode, min, max, &transform);
}

void ModelManager::_glCreateMesh(ModelScene *scene) {
	for (U32 i = 0; i < scene->scene->mNumMeshes; i++) {
		const aiMesh *mesh = scene->scene->mMeshes[i];

		Material *material = nullptr;
		if (!_generateMaterial(scene, mesh, material));
			continue;

		ModelScene::ModelMesh modelMesh;
		memset(&modelMesh, 0, sizeof(ModelScene::ModelMesh));
		modelMesh.material = material;

		std::vector<ModelVertex> vertList(mesh->mNumVertices);
		for (U32 j = 0; j < mesh->mNumVertices; j++) {
			ModelVertex vertex;
			vertex.position = mesh->mVertices[j];
			vertex.normal = mesh->mNormals[j];
			vertex.textureCoords = glm::vec2(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y);
			vertex.tangent = mesh->mTangents[j];
			vertex.bitangent = mesh->mBitangents[j];

			vertList[j] = vertex;
		}

		// now, upload the vertices to the GL
		glGenBuffers(1, &modelMesh.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, modelMesh.vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(ModelVertex) * mesh->mNumVertices, &vertList[0], GL_STATIC_DRAW);

		// get primitive type
		switch (mesh->mPrimitiveTypes) {
		case aiPrimitiveType_POINT:
			modelMesh.primitive = 1;
			break;
		case aiPrimitiveType_LINE:
			modelMesh.primitive = 2;
			break;
		case aiPrimitiveType_TRIANGLE:
			modelMesh.primitive = 3;
			break;
		default:
			// invalid primitive type
			assert(false);
			break;
		}

		// now, do the index buffer
		std::vector<U16> indices;
		for (U32 j = 0; j < mesh->mNumFaces; j++) {
			for (U32 k = 0; k < modelMesh.primitive; k++) {
				indices.push_back(mesh->mFaces[j].mIndices[k]);
			}
		}
		glGenBuffers(1, &modelMesh.ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelMesh.ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(U16) * indices.size(), &indices, GL_STATIC_DRAW);

		modelMesh.numIndices = static_cast<U16>(indices.size());

		scene->meshes.push_back(modelMesh);
	}

	// clear the GL states
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

bool ModelManager::_generateMaterial(const ModelScene *scene, const aiMesh *mesh, Material *material) {
	aiString path;
	aiMaterial *aiMat = scene->scene->mMaterials[mesh->mMaterialIndex];
	if (aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &path) != AI_SUCCESS) {
		// failed to load a texture
		printf("Failed to load texture at path: %s", path.C_Str());
		material = nullptr;
		return false;
	}
	std::string directory = IO::getPath(std::string(path.C_Str()));
	std::string materialName = IO::getName(std::string(path.C_Str()));
	std::string diffuseFile = Texture::find(directory + DIR_SEP + materialName);

	//If we can't find it, just chuck the lot and keep going.
	if (!diffuseFile.length()) {
		fprintf(stderr, "Error in reading bitmap: %s Bitmap not found.\n", materialName.c_str());
		material = nullptr;
		return false;
	}
	std::string normalName = materialName + ".normal";
	std::string normalFile = Texture::find(directory + DIR_SEP + normalName);
	std::string specularName = materialName + ".alpha";
	std::string specularFile = Texture::find(directory + DIR_SEP + specularName);

	Material *mat = new Material(diffuseFile, normalFile, specularFile);
	material = mat;
	return true;
}