//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _RENDER_MODELMANAGER_H_
#define _RENDER_MODELMANAGER_H_

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <unordered_map>
#include "base/types.h"
#include "render/material.h"

#define MODELMGR ModelManager::getSingleton()

struct ModelVertex {
	aiVector3D position;
	glm::vec2 textureCoords;
	aiVector3D normal;
	aiVector3D tangent;
	aiVector3D bitangent;
};

class Shape;

class ModelManager {
private:
	struct ModelScene {
		const aiScene *scene;
		aiVector3D sceneCenter;
		aiVector3D sceneMin;
		aiVector3D sceneMax;

		struct ModelMesh {
			GLuint vbo;
			GLuint ibo;
			U16 numIndices;
			GLenum primitive;
			Material *material;
		};
		std::vector<ModelMesh> meshes;

		ModelScene() : scene(nullptr), sceneCenter(0.0f), sceneMin(0.0f), sceneMax(0.0f) {};

		~ModelScene() {
			aiReleaseImport(scene);
		}
	};

	std::unordered_map<std::string, ModelScene*> mResourceCache;

	std::vector<Shape*> mShapeList;

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
	~ModelManager();

	bool loadAsset(const std::string &file);
	void addShape(Shape *shape) {
		mShapeList.push_back(shape);
	}
	bool releaseAsset(const std::string &file);
	bool containsModel(const std::string &file) const;
	void render(Shader *shader, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix);

	static ModelManager* getSingleton() {
		static ModelManager *manager = new ModelManager();
		return manager;
	}
};

#endif 