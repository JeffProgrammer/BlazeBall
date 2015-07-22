//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
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

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "game/gameInterior.h"
#include "base/math.h"

void GameInterior::init() {
	Triangle **perMaterialTriangles = new Triangle*[materialName.size()];
	
	U32 *materialTriangles = new U32[materialName.size()];
	renderInfo.numMaterialTriangles = new U32[materialName.size()];
	
	U32 numTriangles = 0;
	
	for (U32 i = 0; i < materialName.size(); i ++) {
		renderInfo.numMaterialTriangles[i] = 0;
	}
	
	//Generate our triangle mesh for rendering
	for (U32 i = 0; i < surface.size(); i ++) {
		Surface surface = this->surface[i];
		renderInfo.numMaterialTriangles[surface.textureIndex] += surface.windingCount - 2;
		numTriangles += surface.windingCount - 2;
	}
	
	//Load all the textures
	for (U32 i = 0; i < materialName.size(); i ++) {
		perMaterialTriangles[i] = new Triangle[renderInfo.numMaterialTriangles[i]];
		materialTriangles[i] = 0;
		
		Material *mat = material[i];
		if (mat) {
			mat->generate();
		}
	}
	noise->generateBuffer();
	
	Point3F *tangentMap = new Point3F[normal.size()];
	Point3F *bitangentMap = new Point3F[normal.size()];
	
	for (U32 i = 0; i < normal.size(); i ++) {
		tangentMap[i] = Point3F();
		bitangentMap[i] = Point3F();
	}
	
	for (U32 i = 0; i < surface.size(); i ++) {
		Surface surface = this->surface[i];
		
		Point3F normal = this->normal[plane[surface.planeIndex].normalIndex];
		if (surface.planeFlipped) {
			normal *= -1;
		}
		
		//			Point3F color((F32)surface.planeIndex / (F32)numPlanes, (F32)(surface.planeIndex % 32)/32.f, (F32)(surface.planeIndex % 8)/8.f);
		
		//New and improved rendering with actual Triangle Strips this time
		for (U32 j = surface.windingStart + 2; j < surface.windingStart + surface.windingCount; j ++) {
			Point3F v0, v1, v2;
			
			if ((j - (surface.windingStart + 2)) % 2 == 0) {
				v0 = point[index[j]];
				v1 = point[index[j - 1]];
				v2 = point[index[j - 2]];
			} else {
				v0 = point[index[j - 2]];
				v1 = point[index[j - 1]];
				v2 = point[index[j]];
			}
			
			TexGenEq texGenEq = this->texGenEq[surface.texGenIndex];
			
			Point2F uv0 = Point2F(planeF_distance_to_point(texGenEq.planeX, v0), planeF_distance_to_point(texGenEq.planeY, v0));
			Point2F uv1 = Point2F(planeF_distance_to_point(texGenEq.planeX, v1), planeF_distance_to_point(texGenEq.planeY, v1));
			Point2F uv2 = Point2F(planeF_distance_to_point(texGenEq.planeX, v2), planeF_distance_to_point(texGenEq.planeY, v2));
			
			//				U32 plane = this->plane[surface.planeIndex].normalIndex;
			//				Point3F tangent;
			//				Point3F bitangent;
			//				if (tangentMap[plane].length()) {
			//					tangent = tangentMap[plane];
			//					bitangent = bitangentMap[plane];
			//				} else {
			Point3F deltaPos1 = v1 - v0;
			Point3F deltaPos2 = v2 - v0;
			Point2F deltaUV1 = uv1 - uv0;
			Point2F deltaUV2 = uv2 - uv0;
			
			F32 r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
			
			Point3F tangent   = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
			Point3F bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;
			
			tangent = (tangent - (normal * normal.dot(tangent))).normalize();
			if ((normal.cross(tangent)).dot(bitangent) < 0.0f) {
				tangent *= -1.0f;
			}
			
			tangent = tangent.normalize();
			bitangent = bitangent.normalize();
			
			//					tangentMap[plane] = tangent;
			//					bitangentMap[plane] = bitangent;
			//				}
			
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
	for (U32 i = 0; i < materialName.size(); i ++) {
		for (U32 j = 0; j < materialTriangles[i]; j ++) {
			triangle[numTriangles] = perMaterialTriangles[i][j];
			numTriangles ++;
		}
		delete [] perMaterialTriangles[i];
	}
	delete [] perMaterialTriangles;
	
	//Generate us a vertex buffer
	glGenBuffers(1, &renderInfo.vertexBuffer);
	
	//Use the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, renderInfo.vertexBuffer);
	
	//Upload the buffer data to OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle) * numTriangles, &triangle[0], GL_STATIC_DRAW);
	
	renderInfo.generated = true;
}

void GameInterior::render() {
	if (!renderInfo.generated)
		init();
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER, renderInfo.vertexBuffer);

	//0th array - vertices
	glVertexAttribPointer(0, //Attribute 0
						  3, //3 components
						  GL_FLOAT, //Type
						  GL_FALSE, //Normalized
						  sizeof(Vertex), //Stride
						  (void *)offsetof(Vertex, point)); //Offset

	//1st array - uvs
	glVertexAttribPointer(1, //Attribute 1
						  2, //2 components
						  GL_FLOAT, //Type
						  GL_FALSE, //Normalized
						  sizeof(Vertex), //Stride
						  (void *)offsetof(Vertex, uv)); //Offset

	//2nd array - normals
	glVertexAttribPointer(2, //Attribute 2
						  3, //3 components
						  GL_FLOAT, //Type
						  GL_FALSE, //Normalized
						  sizeof(Vertex), //Stride
						  (void *)offsetof(Vertex, normal)); //Offset

	//3rd array - tangents
	glVertexAttribPointer(3, //Attribute 3
						  3, //3 components
						  GL_FLOAT, //Type
						  GL_FALSE, //Normalized
						  sizeof(Vertex), //Stride
						  (void *)offsetof(Vertex, tangent)); //Offset

	//4th array - bitangents
	glVertexAttribPointer(4, //Attribute 4
						  3, //3 components
						  GL_FLOAT, //Type
						  GL_FALSE, //Normalized
						  sizeof(Vertex), //Stride
						  (void *)offsetof(Vertex, bitangent)); //Offset

	U32 start = 0;
	for (U32 i = 0; i < materialName.size(); i ++) {
		if (renderInfo.numMaterialTriangles[i] > 0) {
			if (material[i]) {
				material[i]->activate();
				noise->activate();
			} else {
				printf("Trying to render with invalid material %d: %s\n", i, materialName[i].c_str());
			}
			glDrawArrays(GL_TRIANGLES, start * 3, renderInfo.numMaterialTriangles[i] * 3);
			start += renderInfo.numMaterialTriangles[i];
			if (material[i]) {
				noise->deactivate();
				material[i]->deactivate();
			}
		}
	}

	//Disable arrays
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}
