//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//
// Note: portions of this code is from the assimp sample code:
// https://github.com/assimp/assimp/blob/master/samples/SimpleOpenGL/Sample_SimpleOpenGL.c
// https://github.com/assimp/assimp/tree/master/samples/SimpleAssimpViewX
//------------------------------------------------------------------------------

#include "game/Shape.h"
#include "render/modelManager.h"

// For getcwd
#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd
#else
#include <unistd.h>
#endif

IMPLEMENT_SCRIPTOBJECT(Shape, RenderedObject);

Shape::Shape(const std::string &shapeFile) : RenderedObject() {
	mShapeFile = shapeFile;
	mGenerated = false;

	char buffer[512]; // If your path is longer than this, then FUCK YOU
	mCurrentWorkingDir = getcwd(buffer, 512);
}

Shape::~Shape() {
	for (size_t i = 0; i < mScene.meshes.size(); i++) {
		glDeleteBuffers(1, &mScene.meshes[i].vbo);
		glDeleteBuffers(1, &mScene.meshes[i].ibo);
	}
}

void Shape::updateTick(const F64 &delta) {

}

void Shape::render(RenderInfo &info) {
	if (!mGenerated) {
		loadShape();
		mGenerated = true;
	}

	for (const auto &mesh : mScene.meshes) {
		RenderUserData *data = new RenderUserData;
		data->indiceCount = mesh.numIndices;
		data->offset = 0;
		data->primitive = mesh.primitive;
		data->ibo = mesh.ibo;
		data->vbo = mesh.vbo;
		info.addRenderMethod(mesh.material, RenderInfo::RenderMethod::from_method<Shape, &Shape::draw>(this), data);
	}
}

void Shape::draw(Material *material, ::RenderInfo &info, void *userData) {
	RenderUserData *data = static_cast<RenderUserData*>(userData);

	Shader *shader = material->getShader();
	loadModelMatrix(info, shader);

	glBindBuffer(GL_ARRAY_BUFFER, data->vbo);
	shader->enableAttributes();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->ibo);
	glDrawElements(data->primitive, data->indiceCount, GL_UNSIGNED_SHORT, reinterpret_cast<void*>(data->offset));

	shader->disableAttributes();

	GL_CHECKERRORS();
	delete data;
}

void Shape::loadShape() {
	const aiScene *scene = aiImportFile(mShapeFile.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene == nullptr)
		return;

	mScene.scene = scene;

	// get scene center:
	_getBoundingBox(scene, &mScene.sceneMin, &mScene.sceneMax);
	mScene.sceneCenter.x = (mScene.sceneMin.x + mScene.sceneMax.x) / 2.0f;
	mScene.sceneCenter.y = (mScene.sceneMin.y + mScene.sceneMax.y) / 2.0f;
	mScene.sceneCenter.z = (mScene.sceneMin.z + mScene.sceneMax.z) / 2.0f;

	_glCreateMesh(&mScene);
}

void Shape::_getBoundingBoxNode(const aiScene *scene, const aiNode *node, aiVector3D *min, aiVector3D *max, aiMatrix4x4 *transform) {
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

void Shape::_getBoundingBox(const aiScene *scene, aiVector3D *min, aiVector3D *max) {
	aiMatrix4x4 transform;
	aiIdentityMatrix4(&transform);

	min->x = min->y = min->z = 1e10f;
	max->x = max->y = max->z = -1e10f;
	_getBoundingBoxNode(scene, scene->mRootNode, min, max, &transform);
}

void Shape::_glCreateMesh(ModelScene *scene) {
	for (U32 i = 0; i < scene->scene->mNumMeshes; i++) {
		const aiMesh *mesh = scene->scene->mMeshes[i];

		Material *material = nullptr;
		if (!_generateMaterial(scene, mesh, material))
			continue;

		ModelScene::ModelMesh modelMesh;
		memset(&modelMesh, 0, sizeof(ModelScene::ModelMesh));
		modelMesh.material = material;

		std::vector<Vertex> vertList(mesh->mNumVertices);
		for (U32 j = 0; j < mesh->mNumVertices; j++) {
			Vertex vertex;
			vertex.point  = assimpCast<glm::vec3>(mesh->mVertices[j]);
			vertex.normal = assimpCast<glm::vec3>(mesh->mNormals[j]);
			vertex.uv     = glm::vec2(mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y);

			if (mesh->HasTangentsAndBitangents()) {
				vertex.tangent = assimpCast<glm::vec3>(mesh->mTangents[j]);
				vertex.bitangent = assimpCast<glm::vec3>(mesh->mBitangents[j]);
			} else {
				vertex.tangent = glm::vec3(0.0f);
				vertex.bitangent = glm::vec3(0.0f);
			}

			vertList[j] = vertex;
		}

		// now, upload the vertices to the GL
		glGenBuffers(1, &modelMesh.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, modelMesh.vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh->mNumVertices, &vertList[0], GL_STATIC_DRAW);

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

bool Shape::_generateMaterial(const ModelScene *scene, const aiMesh *mesh, Material *&material) {
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
		IO::printf("Error in reading bitmap: %s Bitmap not found.\n", materialName.c_str());
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

void Shape::initFields() {
	AddField(Shape::mShapeFile, "shapeFile");
}