//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <glm/ext.hpp>
#include "game/gameInterior.h"
#include "base/math.h"
#include "render/util.h"
#include "render/scene.h"

void GameInterior::init() {
	std::vector<std::vector<Triangle>> perMaterialTriangles(mInterior.materialName.size());
	std::vector<U32> materialTriangles(mInterior.materialName.size());

	U32 numTriangles = 0;
	renderInfo.numMaterialTriangles = std::vector<U32>(mInterior.surface.size());

	//Generate our triangle mesh for rendering
	for (U32 i = 0; i < mInterior.surface.size(); i ++) {
		DIF::Interior::Surface surface = mInterior.surface[i];
		renderInfo.numMaterialTriangles[surface.textureIndex] += surface.windingCount - 2;
		numTriangles += surface.windingCount - 2;
	}
	
	//Load all the textures
	for (U32 i = 0; i < mInterior.materialName.size(); i ++) {
		perMaterialTriangles[i] = std::vector<Triangle>(renderInfo.numMaterialTriangles[i]);
	}

	std::vector<glm::vec3> tangentMap(mInterior.normal.size());
	std::vector<glm::vec3> bitangentMap(mInterior.normal.size());
	
	for (U32 i = 0; i < mInterior.surface.size(); i ++) {
		DIF::Interior::Surface surface = mInterior.surface[i];
		
		glm::vec3 normal = mInterior.normal[mInterior.plane[surface.planeIndex].normalIndex];
		if (surface.planeFlipped) {
			normal *= -1;
		}

		//New and improved rendering with actual Triangle Strips this time
		for (U32 j = surface.windingStart + 2; j < surface.windingStart + surface.windingCount; j ++) {
			glm::vec3 v0, v1, v2;
			
			if ((j - (surface.windingStart + 2)) % 2 == 0) {
				v0 = mInterior.point[mInterior.index[j]];
				v1 = mInterior.point[mInterior.index[j - 1]];
				v2 = mInterior.point[mInterior.index[j - 2]];
			} else {
				v0 = mInterior.point[mInterior.index[j - 2]];
				v1 = mInterior.point[mInterior.index[j - 1]];
				v2 = mInterior.point[mInterior.index[j]];
			}
			
			DIF::Interior::TexGenEq texGenEq = mInterior.texGenEq[surface.texGenIndex];
			
			glm::vec2 uv0 = glm::vec2(planeF_distance_to_point(texGenEq.planeX, v0), planeF_distance_to_point(texGenEq.planeY, v0));
			glm::vec2 uv1 = glm::vec2(planeF_distance_to_point(texGenEq.planeX, v1), planeF_distance_to_point(texGenEq.planeY, v1));
			glm::vec2 uv2 = glm::vec2(planeF_distance_to_point(texGenEq.planeX, v2), planeF_distance_to_point(texGenEq.planeY, v2));
			
			glm::vec3 deltaPos1 = v1 - v0;
			glm::vec3 deltaPos2 = v2 - v0;
			glm::vec2 deltaUV1 = uv1 - uv0;
			glm::vec2 deltaUV2 = uv2 - uv0;
			
			F32 r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
			
			glm::vec3 tangent   = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
			glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;
			
			tangent = glm::normalize(tangent - (normal * glm::dot(normal, tangent)));
			if (glm::dot(glm::cross(normal, tangent), bitangent) < 0.0f) {
				tangent *= -1.0f;
			}
			
			tangent = glm::normalize(tangent);
			bitangent = glm::normalize(bitangent);

			perMaterialTriangles[surface.textureIndex][materialTriangles[surface.textureIndex]].verts[0].point = v0;
			perMaterialTriangles[surface.textureIndex][materialTriangles[surface.textureIndex]].verts[1].point = v1;
			perMaterialTriangles[surface.textureIndex][materialTriangles[surface.textureIndex]].verts[2].point = v2;
			perMaterialTriangles[surface.textureIndex][materialTriangles[surface.textureIndex]].verts[0].uv = uv0;
			perMaterialTriangles[surface.textureIndex][materialTriangles[surface.textureIndex]].verts[1].uv = uv1;
			perMaterialTriangles[surface.textureIndex][materialTriangles[surface.textureIndex]].verts[2].uv = uv2;
			perMaterialTriangles[surface.textureIndex][materialTriangles[surface.textureIndex]].verts[0].normal = normal;
			perMaterialTriangles[surface.textureIndex][materialTriangles[surface.textureIndex]].verts[1].normal = normal;
			perMaterialTriangles[surface.textureIndex][materialTriangles[surface.textureIndex]].verts[2].normal = normal;
			perMaterialTriangles[surface.textureIndex][materialTriangles[surface.textureIndex]].verts[0].tangent = tangent;
			perMaterialTriangles[surface.textureIndex][materialTriangles[surface.textureIndex]].verts[1].tangent = tangent;
			perMaterialTriangles[surface.textureIndex][materialTriangles[surface.textureIndex]].verts[2].tangent = tangent;
			perMaterialTriangles[surface.textureIndex][materialTriangles[surface.textureIndex]].verts[0].bitangent = bitangent;
			perMaterialTriangles[surface.textureIndex][materialTriangles[surface.textureIndex]].verts[1].bitangent = bitangent;
			perMaterialTriangles[surface.textureIndex][materialTriangles[surface.textureIndex]].verts[2].bitangent = bitangent;
			materialTriangles[surface.textureIndex] ++;
		}
	}
	
	std::vector<Triangle> triangle(numTriangles);
	
	//Load all the textures
	numTriangles = 0;
	for (U32 i = 0; i < mInterior.materialName.size(); i ++) {
		for (U32 j = 0; j < materialTriangles[i]; j ++) {
			triangle[numTriangles] = perMaterialTriangles[i][j];
			numTriangles ++;
		}
	}

	//Upload the buffer data to OpenGL
	glGenBuffers(1, &mVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle) * numTriangles, &triangle[0], GL_STATIC_DRAW);
	
	renderInfo.generated = true;
}

void GameInterior::render(const ::RenderInfo &info) {
	if (!renderInfo.generated)
		init();

	//TODO: Nasty hack because we init interiors before the shaders are loaded
	if (mMaterial->getShader() == Shader::defaultShader || mMaterial->getShader() == nullptr)
		mMaterial->setShader(Scene::getSingleton()->mInteriorShader);

	//TODO: Don't load the materials like this, but instead per-material in the
	// render loop
	mMaterial->activate();

	Shader *shader = mMaterial->getShader();
	info.loadShader(shader);

	loadModelMatrix(info, shader);

	// bind vbo
	glBindBuffer(GL_ARRAY_BUFFER, mVbo);
	shader->enableAttributes();

	shader->setUniform("reflectivity", 0.f);
	GL_CHECKERRORS();

	U32 start = 0;
	for (U32 i = 0; i < mInterior.materialName.size(); i ++) {
		if (renderInfo.numMaterialTriangles[i] > 0) {
			if (mMaterialList[i]) {
				mMaterialList[i]->activate();
				mNoiseTexture->activate(GL_TEXTURE3);
				GL_CHECKERRORS();
			} else {
				printf("Trying to render with invalid material %d: %s\n", i, mInterior.materialName[i].c_str());
			}
			glDrawArrays(GL_TRIANGLES, start * 3, renderInfo.numMaterialTriangles[i] * 3);
			GL_CHECKERRORS();
			start += renderInfo.numMaterialTriangles[i];
			if (mMaterialList[i]) {
				mNoiseTexture->deactivate();
				mMaterialList[i]->deactivate();
				GL_CHECKERRORS();
			}
		}
	}
	GL_CHECKERRORS();

	shader->disableAttributes();
	mMaterial->deactivate();
}
