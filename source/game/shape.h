//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GAME_SHAPE_H_
#define _GAME_SHAPE_H_

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "game/gameObject.h"
#include "render/renderInfo.h"
#include "renderedObject.h"

class Shape : public RenderedObject {
	typedef RenderedObject Parent;
protected:
	std::string mShapeFile;

	bool mGenerated;

	std::string mCurrentWorkingDir;

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

	ModelScene mScene;

	struct RenderUserData {
		U16 indiceCount;
		U32 offset;
		GLenum primitive;
		GLuint vbo;
		GLuint ibo;
	};

	DECLARE_SCRIPTOBJECT(Shape);

public:
	Shape(const std::string &shapeFile);
	virtual ~Shape();
	virtual void updateTick(const F64 &delta) override;
	virtual void render(RenderInfo &info) override;

	void loadShape();
	void draw(Material *material, ::RenderInfo &info, void *userData);

	void _getBoundingBoxNode(const aiScene *scene, const aiNode *node, aiVector3D *min, aiVector3D *max, aiMatrix4x4 *transform);
	void _getBoundingBox(const aiScene *scene, aiVector3D *min, aiVector3D *max);

	void _glCreateMesh(ModelScene *scene);

	/// Generates the materials for this specific mesh.
	/// @param IN scene The model scene object of which we are generating materials for.
	/// @param IN mesh The mesh object that the material is for.
	/// @param OUT material The material object that will be used for this mesh, or nullptr if failure.
	/// @return true if the materials could be created, false if the material could not be created.
	bool _generateMaterial(const ModelScene *scene, const aiMesh *mesh, Material *&material);

	const std::string& getShapeFile() const {
		return mShapeFile;
	}

	static void initFields();

	template<typename To, typename From>
	To assimpCast(const From &vec);
};

template<>
inline glm::vec3 Shape::assimpCast(const aiVector3D &vec) {
	return glm::vec3(vec.x, vec.y, vec.z);
}

template<>
inline glm::vec2 Shape::assimpCast(const aiVector2D &vec) {
	return glm::vec2(vec.x, vec.y);
}

#endif // _GAME_SHAPE_H_
