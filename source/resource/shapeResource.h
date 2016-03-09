//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _RESOURCE_SHAPERESOURCE_H_
#define _RESOURCE_SHAPERESOURCE_H_

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "resource/MeshResource.h"

class ShapeResource : public MeshResource {
	typedef MeshResource Parent;
public:
	struct SubMeshData {
		std::vector<U16> indices;
		std::vector<Vertex> vertices;
		GLenum primitive;
		Material *material; // Todo make materials into a resource
	};

	ShapeResource(const std::string &file);
	~ShapeResource();

	virtual bool load() override;

	inline const BoxF getWorldBox() const {
		return BoxF(mAssimpScene.sceneMin, mAssimpScene.sceneMax);
	}

private:
	struct AssimpScene {
		const aiScene *scene;
		glm::vec3 sceneMin;
		glm::vec3 sceneMax;
	};
	AssimpScene mAssimpScene;

	std::vector<SubMeshData> mSubMeshes;

protected:
	void _getBoundingBoxNode(const aiScene *scene, const aiNode *node, aiVector3D *min, aiVector3D *max, aiMatrix4x4 *transform);
	void _getBoundingBox(const aiScene *scene, aiVector3D *min, aiVector3D *max);

	void _readMesh();

	// TODO: MAKE MATERIALS INTO A RESOURCE.
	/// Generates the materials for this specific mesh.
	/// @param IN scene The model scene object of which we are generating materials for.
	/// @param IN mesh The mesh object that the material is for.
	/// @param OUT material The material object that will be used for this mesh, or nullptr if failure.
	/// @return true if the materials could be created, false if the material could not be created.
	bool _generateMaterial(const aiMesh *mesh, Material *&material);

	template<typename To, typename From>
	To assimpCast(const From &vec);
};

template<>
inline glm::vec3 ShapeResource::assimpCast(const aiVector3D &vec) {
	return glm::vec3(vec.x, vec.y, vec.z);
}

template<>
inline glm::vec2 ShapeResource::assimpCast(const aiVector2D &vec) {
	return glm::vec2(vec.x, vec.y);
}


#endif // _RESOURCE_MESHRESOURCE_H_
