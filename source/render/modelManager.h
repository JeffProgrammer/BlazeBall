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
//------------------------------------------------------------------------------

#ifndef _RENDER_MODELMANAGER_H_
#define _RENDER_MODELMANAGER_H_

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <unordered_map>
#include "base/types.h"
#include "render/material.h"
#include "graphics/vertexBufferObject.h"

struct ModelVertex {
	aiVector3D position;
	glm::vec2 textureCoords;
	aiVector3D normal;
	aiVector3D tangent;
	aiVector3D bitangent;
};


class ModelManager {
private:
	struct ModelScene {
		const aiScene *scene;
		aiVector3D sceneCenter;
		aiVector3D sceneMin;
		aiVector3D sceneMax;

		struct ModelMesh {
			VertexBufferObject *vbo;
			GLuint ibo;
			U16 numIndices;
			GLuint primitive;
			Material *material;
		};
		std::vector<ModelMesh> meshes;

		ModelScene() : scene(nullptr), sceneCenter(0.0f), sceneMin(0.0f), sceneMax(0.0f) {};

		~ModelScene() {
			aiReleaseImport(scene);
		}
	};

	std::unordered_map<std::string, ModelScene*> mResourceCache;

	void _getBoundingBoxNode(const aiScene *scene, const aiNode *node, aiVector3D *min, aiVector3D *max, aiMatrix4x4 *transform);
	void _getBoundingBox(const aiScene *scene, aiVector3D *min, aiVector3D *max);

	void _glCreateMesh(ModelScene *scene);

	/// Generates the materials for this specific mesh.
	/// @param IN scene The model scene object of which we are generating materials for.
	/// @param IN mesh The mesh object that the material is for.
	/// @param OUT material The material object that will be used for this mesh, or nullptr if failure.
	/// @return true if the materials could be created, false if the material could not be created.
	bool _generateMaterial(const ModelScene *scene, const aiMesh *mesh, Material *&material);

	std::string mCurrentWorkingDir;
public:
	ModelManager();

	bool loadAsset(const std::string &file);
	bool releaseAsset(const std::string &file);
	bool containsModel(const std::string &file) const;
	void render();
};

#endif 