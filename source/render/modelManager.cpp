//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//
// Note: portions of this code is from the assimp sample code:
// https://github.com/assimp/assimp/blob/master/samples/SimpleOpenGL/Sample_SimpleOpenGL.c
// https://github.com/assimp/assimp/tree/master/samples/SimpleAssimpViewX
//------------------------------------------------------------------------------

#include "render/modelManager.h"
#include "base/io.h"
#include "render/util.h"
#include "game/Shape.h"

// For getcwd
#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd
#else
#include <unistd.h>
#endif

ModelManager::ModelManager() {
	char buffer[512]; // If your path is longer than this, then FUCK YOU
	mCurrentWorkingDir = getcwd(buffer, 512);
}

ModelManager::~ModelManager() {

}

bool ModelManager::loadAsset(const std::string &file) {
	if (mResourceCache[file])
		return true;

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

void ModelManager::render(Shader *shapeShader, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix) {
	bool firstRender = true;
	GL_CHECKERRORS();

	for (auto shape : mShapeList) {
		auto model = mResourceCache[shape->getShapeFile()];
		const auto &meshes = model->meshes;

		RenderInfo info;
		info.projectionMatrix = projectionMatrix;
		info.viewMatrix = viewMatrix;

		Mat4 modelMatrix = Mat4(1.0f);
		shape->calculateModelMatrix(info, modelMatrix);
		shapeShader->setUniformMatrix("modelMat", GL_FALSE, modelMatrix);

		for (const auto &mesh : meshes) {
			mesh.material->activate();

			glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

			// for now, this will cache these, as each VBO currently uses the same offsets
			// note: a VBO must be bound before the layout can be bound
			if (firstRender) {
				firstRender = false;
				glEnableVertexAttribArray(shapeShader->getAttributeLocation("vertexPosition"));
				glEnableVertexAttribArray(shapeShader->getAttributeLocation("vertexUV"));
				glVertexAttribPointer(shapeShader->getAttributeLocation("vertexPosition"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(ModelVertex, position)));
				glVertexAttribPointer(shapeShader->getAttributeLocation("vertexUV"), 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(ModelVertex, textureCoords)));
				GL_CHECKERRORS();
			}

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
			glDrawElements(mesh.primitive, mesh.numIndices, GL_UNSIGNED_SHORT, reinterpret_cast<void*>(0));
		}
		GL_CHECKERRORS();
	}

	//Disable arrays if any models were drawn.
	if (firstRender == false) {
		glDisableVertexAttribArray(shapeShader->getAttributeLocation("vertexPosition"));
		glDisableVertexAttribArray(shapeShader->getAttributeLocation("vertexUV"));
	}
	GL_CHECKERRORS();
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
		if (!_generateMaterial(scene, mesh, material))
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

			if (mesh->HasTangentsAndBitangents()) {
				vertex.tangent = mesh->mTangents[j];
				vertex.bitangent = mesh->mBitangents[j];
			} else {
				vertex.tangent = aiVector3D(0.0f);
				vertex.bitangent = aiVector3D(0.0f);
			}

			vertList[j] = vertex;
		}

		// now, upload the vertices to the GL
		glGenBuffers(1, &modelMesh.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, modelMesh.vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(ModelVertex) * mesh->mNumVertices, &vertList[0], GL_STATIC_DRAW);

		// get primitive type
		U32 primCount = 0;
		switch (mesh->mPrimitiveTypes) {
		case aiPrimitiveType_POINT:
			modelMesh.primitive = GL_POINTS;
			primCount = 1;
			break;
		case aiPrimitiveType_LINE:
			modelMesh.primitive = GL_LINES;
			primCount = 2;
			break;
		case aiPrimitiveType_TRIANGLE:
			modelMesh.primitive = GL_TRIANGLES;
			primCount = 3;
			break;
		default:
			// invalid primitive type
			assert(false);
			break;
		}

		// now, do the index buffer
		std::vector<U16> indices(mesh->mNumFaces * primCount);
		int index = 0;
		for (U32 j = 0; j < mesh->mNumFaces; j++) {
			for (U32 k = 0; k < primCount; k++) {
				indices[index] = mesh->mFaces[j].mIndices[k];
				index++;
			}
		}
		glGenBuffers(1, &modelMesh.ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, modelMesh.ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(U16) * indices.size(), &indices[0], GL_STATIC_DRAW);
		modelMesh.numIndices = static_cast<U16>(indices.size());

		scene->meshes.push_back(modelMesh);
	}
}

bool ModelManager::_generateMaterial(const ModelScene *scene, const aiMesh *mesh, Material *&material) {
	aiString path;
	aiMaterial *aiMat = scene->scene->mMaterials[mesh->mMaterialIndex];
	if (aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &path) != AI_SUCCESS) {
		// failed to load a texture
		IO::printf("Failed to load texture at path: %s\n", path.C_Str());
		material = nullptr;
		return false;
	}
	std::string directory = mCurrentWorkingDir + DIR_SEP + IO::getPath(std::string(path.C_Str()));
	std::string materialName = IO::getBase(std::string(path.C_Str()), '/');
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

	material = new Material(diffuseFile, normalFile, specularFile);
	return true;
}