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
//------------------------------------------------------------------------------

#include "render/modelManager.h"

ModelManager::ModelManager() {

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

	return true;
}

bool ModelManager::releaseAsset(const std::string &file) {
	if (!containsModel(file))
		return false;

	delete mResourceCache[file];
	mResourceCache[file] = nullptr;
	mResourceCache.erase(file);
	return true;
}

bool ModelManager::containsModel(const std::string &file) const {
	return std::find(mResourceCache.begin(), mResourceCache.end(), file) != mResourceCache.end();
}

//------------------------------------------------------------------------------
// private support methods
//------------------------------------------------------------------------------

void ModelManager::_getBoundingBoxNode(const aiScene *scene, const aiNode *node, aiVector3D *min, aiVector3D *max, aiMatrix4x4 *transform) {
	aiMatrix4x4 prev = *transform;
	U32 i, j, numMeshes, numVerts;

	aiMultiplyMatrix4(transform, &node->mTransformation);

	numVerts = node->mNumMeshes;
	for (i = 0; i < numVerts; i++) {
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