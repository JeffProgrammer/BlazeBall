//------------------------------------------------------------------------------
// Copyright (c) 2014 Glenn Smith
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

//Warning: This is the scary file.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include "io.h"
#include "interior.h"
#include "mngsupport.h"
#include "jpegsupport.h"
#include "math.h"

Interior *interior_read_file(FILE *file, String directory) {
	Interior *interior = (Interior *)malloc(sizeof(Interior));

	READTOVAR(interior->interiorFileVersion, U32); //interiorFileVersion
	READTOVAR(interior->detailLevel, U32); //detailLevel
	READTOVAR(interior->minPixels, U32); //minPixels
	READTOVAR(interior->boundingBox, BoxF); //boundingBox
	READTOVAR(interior->boundingSphere, SphereF); //boundingSphere
	READTOVAR(interior->hasAlarmState, U8); //hasAlarmState
	READTOVAR(interior->numLightStateEntries, U32); //numLightStateEntries
	READLOOPVAR(interior->numNormals, interior->normal, Point3F) {
		READTOVAR(interior->normal[i], Point3F); //normal
	}
	READLOOPVAR(interior->numPlanes, interior->plane, Plane) {
		READTOVAR(interior->plane[i].normalIndex, U16); //normalIndex
		READTOVAR(interior->plane[i].planeDistance, F32); //planeDistance
	}
	READLOOPVAR(interior->numPoints, interior->point, Point3F) {
		READTOVAR(interior->point[i], Point3F); //point
	}
	READLOOPVAR(interior->numPointVisibilities, interior->pointVisibility, U8) {
		READTOVAR(interior->pointVisibility[i], U8); //pointVisibility
	}
	READLOOPVAR(interior->numTexGenEqs, interior->texGenEq, TexGenEq) {
		READTOVAR(interior->texGenEq[i].planeX, PlaneF); //planeX
		READTOVAR(interior->texGenEq[i].planeY, PlaneF); //planeY
	}
	READLOOPVAR(interior->numBSPNodes, interior->BSPNode, BSPNode) {
		READTOVAR(interior->BSPNode[i].planeIndex, U16); //planeIndex
		READTOVAR(interior->BSPNode[i].frontIndex, U16); //frontIndex
		READTOVAR(interior->BSPNode[i].backIndex, U16); //backIndex
	}
	READLOOPVAR(interior->numBSPSolidLeaves, interior->BSPSolidLeaf, BSPSolidLeaf) {
		READTOVAR(interior->BSPSolidLeaf[i].surfaceIndex, U32); //surfaceIndex
		READTOVAR(interior->BSPSolidLeaf[i].surfaceCount, U16); //surfaceCount
	}
	//MaterialList
	READTOVAR(interior->materialListVersion, U8); //version
	READLOOPVAR(interior->numMaterials, interior->material, String) {
		READTOVAR(interior->material[i], String); //material
	}
	READLOOPVAR(interior->numWindings, interior->index, U32) {
		READTOVAR(interior->index[i], U32); //index
	}
	READLOOPVAR(interior->numWindingIndices, interior->windingIndex, WindingIndex) {
		READTOVAR(interior->windingIndex[i].windingStart, U32); //windingStart
		READTOVAR(interior->windingIndex[i].windingCount, U32); //windingCount
	}
	/*
	 Not in MB
	 READLOOP(numEdges, U32) {
	 READ(S32); //pointIndex0
	 READ(S32); //pointIndex1
	 READ(S32); //surfaceIndex0
	 READ(S32); //surfaceIndex1
	 }
	 */
	READLOOPVAR(interior->numZones, interior->zone, Zone) {
		READTOVAR(interior->zone[i].portalStart, U16); //portalStart
		READTOVAR(interior->zone[i].portalCount, U16); //portalCount
		READTOVAR(interior->zone[i].surfaceStart, U32); //surfaceStart
		READTOVAR(interior->zone[i].surfaceCount, U32); //surfaceCount
		/*
		 Not in MB
		 READTOVAR(interior->staticMeshStart[i], U32); //staticMeshStart
		 READTOVAR(interior->staticMeshCount[i], U32); //staticMeshCount
		 READTOVAR(interior->flags[i], U16); //flags
		 */
	}
	READLOOPVAR(interior->numZoneSurfaces, interior->zoneSurface, U16) {
		READTOVAR(interior->zoneSurface[i], U16); //zoneSurface
	}
	READLOOPVAR(interior->numZonePortalList, interior->zonePortalList, U16) {
		READTOVAR(interior->zonePortalList[i], U16); //zonePortalList
	}
	READLOOPVAR(interior->numPortals, interior->portal, Portal) {
		READTOVAR(interior->portal[i].planeIndex, U16); //planeIndex
		READTOVAR(interior->portal[i].triFanCount, U16); //triFanCount
		READTOVAR(interior->portal[i].triFanStart, U32); //triFanStart
		READTOVAR(interior->portal[i].zoneFront, U16); //zoneFront
		READTOVAR(interior->portal[i].zoneBack, U16); //zoneBack
	}
	READLOOPVAR(interior->numSurfaces, interior->surface, Surface) {
		READTOVAR(interior->surface[i].windingStart, U32); //windingStart
		READTOVAR(interior->surface[i].windingCount, U8); //windingCount
		READTOVAR(interior->surface[i].planeIndex, U16); //planeIndex
		READTOVAR(interior->surface[i].textureIndex, U16); //textureIndex
		READTOVAR(interior->surface[i].texGenIndex, U32); //texGenIndex
		READTOVAR(interior->surface[i].surfaceFlags, U8); //surfaceFlags
		READTOVAR(interior->surface[i].fanMask, U32); //fanMask
		{ //LightMap
			READTOVAR(interior->surface[i].lightMap.finalWord, U16); //finalWord
			READTOVAR(interior->surface[i].lightMap.texGenXDistance, F32); //texGenXDistance
			READTOVAR(interior->surface[i].lightMap.texGenYDistance, F32); //texGenYDistance
		}
		READTOVAR(interior->surface[i].lightCount, U16); //lightCount
		READTOVAR(interior->surface[i].lightStateInfoStart, U32); //lightStateInfoStart
		READTOVAR(interior->surface[i].mapOffsetX, U8); //mapOffsetX
		READTOVAR(interior->surface[i].mapOffsetY, U8); //mapOffsetY
		READTOVAR(interior->surface[i].mapSizeX, U8); //mapSizeX
		READTOVAR(interior->surface[i].mapSizeY, U8); //mapSizeY
		/*
		 Not in MB
		 READTOVAR(interior->unused[i], U8); //unused
		 */
	}
	READLOOPVAR(interior->numNormalLMapIndices, interior->normalLMapIndex, U8) {
		READTOVAR(interior->normalLMapIndex[i], U8); //normalLMapIndex
	}
	READLOOPVAR(interior->numAlarmLMapIndices, interior->alarmLMapIndex, U8) {
		READTOVAR(interior->alarmLMapIndex[i], U8); //alarmLMapIndex
	}
	READLOOPVAR(interior->numNullSurfaces, interior->nullSurface, NullSurface) {
		READTOVAR(interior->nullSurface[i].windingStart, U32); //windingStart
		READTOVAR(interior->nullSurface[i].planeIndex, U16); //planeIndex
		READTOVAR(interior->nullSurface[i].surfaceFlags, U8); //surfaceFlags
		READTOVAR(interior->nullSurface[i].windingCount, U8); //windingCount
	}
	READLOOPVAR(interior->numLightMaps, interior->lightMap, LightMap) {
		READTOVAR(interior->lightMap[i].lightMap, PNG); //lightMap
		/*
		 Not in MB
		 READTOVAR(interior->lightDirMap[i], PNG); //lightDirMap
		 */
		READTOVAR(interior->lightMap[i].keepLightMap, U8); //keepLightMap
	}
	READLOOPVAR(interior->numSolidLeafSurfaces, interior->solidLeafSurface, U32) {
		READTOVAR(interior->solidLeafSurface[i], U32); //solidLeafSurface
	}
	READLOOPVAR(interior->numAnimatedLights, interior->animatedLight, AnimatedLight) {
		READTOVAR(interior->animatedLight[i].nameIndex, U32); //nameIndex
		READTOVAR(interior->animatedLight[i].stateIndex, U32); //stateIndex
		READTOVAR(interior->animatedLight[i].stateCount, U16); //stateCount
		READTOVAR(interior->animatedLight[i].flags, U16); //flags
		READTOVAR(interior->animatedLight[i].duration, U32); //duration
	}
	READLOOPVAR(interior->numLightStates, interior->lightState, LightState) {
		READTOVAR(interior->lightState[i].red, U8); //red
		READTOVAR(interior->lightState[i].green, U8); //green
		READTOVAR(interior->lightState[i].blue, U8); //blue
		READTOVAR(interior->lightState[i].activeTime, U32); //activeTime
		READTOVAR(interior->lightState[i].dataIndex, U32); //dataIndex
		READTOVAR(interior->lightState[i].dataCount, U16); //dataCount
	}
	READLOOPVAR(interior->numStateDatas, interior->stateData, StateData) {
		READTOVAR(interior->stateData[i].surfaceIndex, U32); //surfaceIndex
		READTOVAR(interior->stateData[i].mapIndex, U32); //mapIndex
		READTOVAR(interior->stateData[i].lightStateIndex, U16); //lightStateIndex
	}
	READLOOPVAR(interior->numStateDataBuffers, interior->stateDataBuffer, U8) {
		READTOVAR(interior->stateDataBuffer[i], U8); //stateDataBuffer
	}
	READTOVAR(interior->flags, U32); //flags
	READLOOPVAR(interior->numNameBuffers, interior->nameBufferCharacter, U8) {
		READTOVAR(interior->nameBufferCharacter[i], U8); //character
	}
	READLOOP(numSubObjects, U32) {
		//NFC
	}
	READLOOPVAR(interior->numConvexHulls, interior->convexHull, ConvexHull) {
		READTOVAR(interior->convexHull[i].hullStart, U32); //hullStart
		READTOVAR(interior->convexHull[i].hullCount, U16); //hullCount
		READTOVAR(interior->convexHull[i].minX, F32); //minX
		READTOVAR(interior->convexHull[i].maxX, F32); //maxX
		READTOVAR(interior->convexHull[i].minY, F32); //minY
		READTOVAR(interior->convexHull[i].maxY, F32); //maxY
		READTOVAR(interior->convexHull[i].minZ, F32); //minZ
		READTOVAR(interior->convexHull[i].maxZ, F32); //maxZ
		READTOVAR(interior->convexHull[i].surfaceStart, U32); //surfaceStart
		READTOVAR(interior->convexHull[i].surfaceCount, U16); //surfaceCount
		READTOVAR(interior->convexHull[i].planeStart, U32); //planeStart
		READTOVAR(interior->convexHull[i].polyListPlaneStart, U32); //polyListPlaneStart
		READTOVAR(interior->convexHull[i].polyListPointStart, U32); //polyListPointStart
		READTOVAR(interior->convexHull[i].polyListStringStart, U32); //polyListStringStart
		/*
		 Not in MB
		 READTOVAR(interior->staticMesh[i], U8); //staticMesh
		 */
	}
	READLOOPVAR(interior->numConvexHullEmitStrings, interior->convexHullEmitStringCharacter, U8) {
		READTOVAR(interior->convexHullEmitStringCharacter[i], U8); //convexHullEmitStringCharacter
	}
	READLOOPVAR(interior->numHullIndices, interior->hullIndex, U32) {
		READTOVAR(interior->hullIndex[i], U32); //hullIndex
	}
	READLOOPVAR(interior->numHullPlaneIndices, interior->hullPlaneIndex, U16) {
		READTOVAR(interior->hullPlaneIndex[i], U16); //hullPlaneIndex
	}
	READLOOPVAR(interior->numHullEmitStringIndices, interior->hullEmitStringIndex, U32) {
		READTOVAR(interior->hullEmitStringIndex[i], U32); //hullEmitStringIndex
	}
	READLOOPVAR(interior->numHullSurfaceIndices, interior->hullSurfaceIndex, U32) {
		READTOVAR(interior->hullSurfaceIndex[i], U32); //hullSurfaceIndex
	}
	READLOOPVAR(interior->numPolyListPlanes, interior->polyListPlaneIndex, U16) {
		READTOVAR(interior->polyListPlaneIndex[i], U16); //polyListPlaneIndex
	}
	READLOOPVAR(interior->numPolyListPoints, interior->polyListPointIndex, U32) {
		READTOVAR(interior->polyListPointIndex[i], U32); //polyListPointIndex
	}
	READLOOPVAR(interior->numPolyListStrings, interior->polyListStringCharacter, U8) {
		READTOVAR(interior->polyListStringCharacter[i], U8); //polyListStringCharacter
	}
	interior->coordBin = malloc(sizeof(CoordBin) * gNumCoordBins * gNumCoordBins);
	for (U32 i = 0; i < gNumCoordBins * gNumCoordBins; i ++) {
		READTOVAR(interior->coordBin[i].binStart, U32); //binStart
		READTOVAR(interior->coordBin[i].binCount, U32); //binCount
	}
	READLOOPVAR(interior->numCoordBinIndices, interior->coordBinIndex, U16) {
		READTOVAR(interior->coordBinIndex[i], U16); //coordBinIndex
	}
	READTOVAR(interior->coordBinMode, U32); //coordBinMode
	READTOVAR(interior->baseAmbientColor, ColorI); //baseAmbientColor
	READTOVAR(interior->alarmAmbientColor, ColorI); //alarmAmbientColor
	/*
	 There's a long list of static meshes here, but I'm too lazy to add it just to comment it out. Oh and I'd have to implement Point2I / Point2F. See DIF_MB_SPEC.rtf if you want to implement it.
	 */
	READLOOPVAR(interior->numTexNormals, interior->texNormal, Point3F) {
		READTOVAR(interior->texNormal[i], Point3F); //texNormal
	}
	READLOOPVAR(interior->numTexMatrices, interior->texMatrix, TexMatrix) {
		READTOVAR(interior->texMatrix[i].T, S32); //T
		READTOVAR(interior->texMatrix[i].N, S32); //N
		READTOVAR(interior->texMatrix[i].B, S32); //B
	}
	READLOOPVAR(interior->numTexMatIndices, interior->texMatIndex, U32) {
		READTOVAR(interior->texMatIndex[i], U32); //texMatIndex
	}
	if ((READTOVAR(interior->extendedLightMapData, U32))) { //extendedLightMapData
		READTOVAR(interior->lightMapBorderSize, U32); //lightMapBorderSize
		READ(U32); //dummy
	}

	//Allocate all textures for the interior
	if (interior->numMaterials) {
		interior->texture = malloc(sizeof(Texture *) * interior->numMaterials);
		for (U32 i = 0; i < interior->numMaterials; i ++) {
			String material = malloc(sizeof(U8) * strlen((const char *)interior->material[i]) + 1);
			strcpy((char *)material, (const char *)interior->material[i]);
			//Chop off any paths from the material. Constructor likes to save albums in the materials
			// and it royally breaks this program.
			if (strstr((const char *)material, "/")) {
				//Hacky but effective method
				strcpy((char *)material, strstr((const char *)material, "/") + 1);
			}

			BitmapType type = BitmapTypePNG;

			//For some reason these two like to become the same.
			String base = (String)strdup((char *)directory);

			//Allocate enough space in each of these so we can work comfortably
			U32 pathlen = (U32)(strlen((const char *)base) + strlen((const char *)material) + 1);
			String imageFile = malloc(sizeof(U8) * pathlen + 5);

			do {
				//Init imageFile to base/file.png
				pathlen = sprintf((char *)imageFile, "%s/%s.png", base, material);

				type = BitmapTypePNG;

				//If we can't find the PNG, try for JPEG
				//TODO: BMP Support?
				if (!isfile(imageFile)) {
					//Swap the last 3 chars with jpg
					memcpy(imageFile + pathlen - 3, "jpg", 3);
					type = BitmapTypeJPEG;
				}
				//Can't recurse any further
				if (!strrchr((const char *)base, '/'))
					break;

				//If we still can't find it, recurse (hacky but effective method)
				if (!isfile(imageFile))
					*strrchr((const char *)base, '/') = 0;
			} while (!isfile(imageFile) && strcmp((const char *)base, ""));

			//If we can't find it, just chuck the lot and keep going.
			if (!isfile(imageFile)) {
				fprintf(stderr, "Error in reading bitmap: %s Bitmap not found.\n", material);
				interior->texture[i] = NULL;
				free(base);
				free(imageFile);
				continue;
			}

			//Setup
			U8 *bitmap;
			Point2I dims;

			bool (*readFn)(String file, U8 **bitmap, Point2I *dims);

			//Try to read the image based on format
			if (type == BitmapTypePNG)
				readFn = mngReadImage;
			else if (type == BitmapTypeJPEG)
				readFn = jpegReadImage;
			else {
				// ?!
				readFn = mngReadImage;
			}

			if (!readFn(imageFile, &bitmap, &dims)) {
				fprintf(stderr, "Error in reading bitmap: %s Other error\n", imageFile);
				interior->texture[i] = NULL;

				free(bitmap);
				free(base);
				free(imageFile);
				continue;
			}

			//Create a texture from the bitmap (copies bitmap)
			Texture *texture = texture_create_from_pixels(bitmap, dims);
			interior->texture[i] = texture;

			//Clean up bitmap (copied above, this is safe)
			free(bitmap);
			free(base);
			free(imageFile);
		}
	}

	return interior;
}

bool interior_write_file(FILE *file, Interior *interior) {
	WRITECHECK(U32, interior->interiorFileVersion); //interiorFileVersion
	WRITECHECK(U32, interior->detailLevel); //detailLevel
	WRITECHECK(U32, interior->minPixels); //minPixels
	WRITECHECK(BoxF, interior->boundingBox); //boundingBox
	WRITECHECK(SphereF, interior->boundingSphere); //boundingSphere
	WRITECHECK(U8, interior->hasAlarmState); //hasAlarmState
	WRITECHECK(U32, interior->numLightStateEntries); //numLightStateEntries
	WRITELOOPVAR(Point3F, interior->numNormals, interior->normal); //normal
	WRITELOOP(interior->numPlanes) { //numPlanes
		WRITECHECK(U16, interior->plane[i].normalIndex); //normalIndex
		WRITECHECK(F32, interior->plane[i].planeDistance); //planeDistance
	}
	WRITELOOPVAR(Point3F, interior->numPoints, interior->point); //point
	WRITELOOPVAR(U8, interior->numPointVisibilities, interior->pointVisibility); //pointVisibility
	WRITELOOP(interior->numTexGenEqs) { //numTexGenEqs
		WRITECHECK(PlaneF, interior->texGenEq[i].planeX); //planeX
		WRITECHECK(PlaneF, interior->texGenEq[i].planeY); //planeY
	}
	WRITELOOP(interior->numBSPNodes) { //numBSPNodes
		WRITECHECK(U16, interior->BSPNode[i].planeIndex); //planeIndex
		WRITECHECK(U16, interior->BSPNode[i].frontIndex); //frontIndex
		WRITECHECK(U16, interior->BSPNode[i].backIndex); //backIndex
	}
	WRITELOOP(interior->numBSPSolidLeaves) { //numBSPSolidLeaves
		WRITECHECK(U32, interior->BSPSolidLeaf[i].surfaceIndex); //surfaceIndex
		WRITECHECK(U16, interior->BSPSolidLeaf[i].surfaceCount); //surfaceCount
	}
	WRITECHECK(U8, interior->materialListVersion); //materialListVersion
	WRITELOOP(interior->numMaterials) { //numMaterials
		WRITE(String, interior->material[i]); //material
	}
	WRITELOOPVAR(U32, interior->numWindings, interior->index); //index
	WRITELOOP(interior->numWindingIndices) { //numWindingIndices
		WRITECHECK(U32, interior->windingIndex[i].windingStart); //windingStart
		WRITECHECK(U32, interior->windingIndex[i].windingCount); //windingCount
	}
	WRITELOOP(interior->numZones) { //numZones
		WRITECHECK(U16, interior->zone[i].portalStart); //portalStart
		WRITECHECK(U16, interior->zone[i].portalCount); //portalCount
		WRITECHECK(U32, interior->zone[i].surfaceStart); //surfaceStart
		WRITECHECK(U32, interior->zone[i].surfaceCount); //surfaceCount
	}
	WRITELOOPVAR(U16, interior->numZoneSurfaces, interior->zoneSurface); //zoneSurface
	WRITELOOPVAR(U16, interior->numZonePortalList, interior->zonePortalList); //zonePortalList
	WRITELOOP(interior->numPortals) { //numPortals
		WRITECHECK(U16, interior->portal[i].planeIndex); //planeIndex
		WRITECHECK(U16, interior->portal[i].triFanCount); //triFanCount
		WRITECHECK(U32, interior->portal[i].triFanStart); //triFanStart
		WRITECHECK(U16, interior->portal[i].zoneFront); //zoneFront
		WRITECHECK(U16, interior->portal[i].zoneBack); //zoneBack
	}
	WRITELOOP(interior->numSurfaces) { //numSurfaces
		WRITECHECK(U32, interior->surface[i].windingStart); //windingStart
		WRITECHECK(U8, interior->surface[i].windingCount); //windingCount
		WRITECHECK(U16, interior->surface[i].planeIndex); //planeIndex
		WRITECHECK(U16, interior->surface[i].textureIndex); //textureIndex
		WRITECHECK(U32, interior->surface[i].texGenIndex); //texGenIndex
		WRITECHECK(U8, interior->surface[i].surfaceFlags); //surfaceFlags
		WRITECHECK(U32, interior->surface[i].fanMask); //fanMask
		WRITECHECK(U16, interior->surface[i].lightMap.finalWord); //finalWord
		WRITECHECK(F32, interior->surface[i].lightMap.texGenXDistance); //texGenXDistance
		WRITECHECK(F32, interior->surface[i].lightMap.texGenYDistance); //texGenYDistance
		WRITECHECK(U16, interior->surface[i].lightCount); //lightCount
		WRITECHECK(U32, interior->surface[i].lightStateInfoStart); //lightStateInfoStart
		WRITECHECK(U8, interior->surface[i].mapOffsetX); //mapOffsetX
		WRITECHECK(U8, interior->surface[i].mapOffsetY); //mapOffsetY
		WRITECHECK(U8, interior->surface[i].mapSizeX); //mapSizeX
		WRITECHECK(U8, interior->surface[i].mapSizeY); //mapSizeY
	}
	WRITELOOPVAR(U8, interior->numNormalLMapIndices, interior->normalLMapIndex); //normalLMapIndex
	WRITELOOPVAR(U8, interior->numAlarmLMapIndices, interior->alarmLMapIndex); //alarmLMapIndex
	WRITELOOP(interior->numNullSurfaces) { //numNullSurfaces
		WRITECHECK(U32, interior->nullSurface[i].windingStart); //windingStart
		WRITECHECK(U16, interior->nullSurface[i].planeIndex); //planeIndex
		WRITECHECK(U8, interior->nullSurface[i].surfaceFlags); //surfaceFlags
		WRITECHECK(U8, interior->nullSurface[i].windingCount); //windingCount
	}
	WRITELOOP(interior->numLightMaps) { //numLightMaps
		WRITE(PNG, interior->lightMap[i].lightMap); //lightMap
		WRITECHECK(U8, interior->lightMap[i].keepLightMap); //keepLightMap
	}
	WRITELOOPVAR(U32, interior->numSolidLeafSurfaces, interior->solidLeafSurface); //solidLeafSurface
	WRITELOOP(interior->numAnimatedLights) { //numAnimatedLights
		WRITECHECK(U32, interior->animatedLight[i].nameIndex); //nameIndex
		WRITECHECK(U32, interior->animatedLight[i].stateIndex); //stateIndex
		WRITECHECK(U16, interior->animatedLight[i].stateCount); //stateCount
		WRITECHECK(U16, interior->animatedLight[i].flags); //flags
		WRITECHECK(U32, interior->animatedLight[i].duration); //duration
	}
	WRITELOOP(interior->numLightStates) { //numLightStates
		WRITECHECK(U8, interior->lightState[i].red); //red
		WRITECHECK(U8, interior->lightState[i].green); //green
		WRITECHECK(U8, interior->lightState[i].blue); //blue
		WRITECHECK(U32, interior->lightState[i].activeTime); //activeTime
		WRITECHECK(U32, interior->lightState[i].dataIndex); //dataIndex
		WRITECHECK(U16, interior->lightState[i].dataCount); //dataCount
	}
	WRITELOOP(interior->numStateDatas) { //numStateDatas
		WRITECHECK(U32, interior->stateData[i].surfaceIndex); //surfaceIndex
		WRITECHECK(U32, interior->stateData[i].mapIndex); //mapIndex
		WRITECHECK(U16, interior->stateData[i].lightStateIndex); //lightStateIndex
	}
	WRITELOOPVAR(U32, interior->numStateDataBuffers, interior->stateDataBuffer); //stateDataBuffer
	WRITECHECK(U32, interior->flags); //flags
	WRITELOOPVAR(S8, interior->numNameBuffers, interior->nameBufferCharacter); //nameBufferCharacter
	WRITELOOP(interior->numSubObjects) { //numSubObjects

	}
	WRITELOOP(interior->numConvexHulls) { //numConvexHulls
		WRITECHECK(U32, interior->convexHull[i].hullStart); //hullStart
		WRITECHECK(U16, interior->convexHull[i].hullCount); //hullCount
		WRITECHECK(F32, interior->convexHull[i].minX); //minX
		WRITECHECK(F32, interior->convexHull[i].maxX); //maxX
		WRITECHECK(F32, interior->convexHull[i].minY); //minY
		WRITECHECK(F32, interior->convexHull[i].maxY); //maxY
		WRITECHECK(F32, interior->convexHull[i].minZ); //minZ
		WRITECHECK(F32, interior->convexHull[i].maxZ); //maxZ
		WRITECHECK(U32, interior->convexHull[i].surfaceStart); //surfaceStart
		WRITECHECK(U16, interior->convexHull[i].surfaceCount); //surfaceCount
		WRITECHECK(U32, interior->convexHull[i].planeStart); //planeStart
		WRITECHECK(U32, interior->convexHull[i].polyListPlaneStart); //polyListPlaneStart
		WRITECHECK(U32, interior->convexHull[i].polyListPointStart); //polyListPointStart
		WRITECHECK(U32, interior->convexHull[i].polyListStringStart); //polyListStringStart
	}
	WRITELOOPVAR(U8, interior->numConvexHullEmitStrings, interior->convexHullEmitStringCharacter); //convexHullEmitStringCharacter
	WRITELOOPVAR(U32, interior->numHullIndices, interior->hullIndex); //hullIndex
	WRITELOOPVAR(U16, interior->numHullPlaneIndices, interior->hullPlaneIndex); //hullPlaneIndex
	WRITELOOPVAR(U32, interior->numHullEmitStringIndices, interior->hullEmitStringIndex); //hullEmitStringIndex
	WRITELOOPVAR(U32, interior->numHullSurfaceIndices, interior->hullSurfaceIndex); //hullSurfaceIndex
	WRITELOOPVAR(U16, interior->numPolyListPlanes, interior->polyListPlaneIndex); //polyListPlaneIndex
	WRITELOOPVAR(U32, interior->numPolyListPoints, interior->polyListPointIndex); //polyListPointIndex
	WRITELOOPVAR(U8, interior->numPolyListStrings, interior->polyListStringCharacter); //polyListStringCharacter
	for (U32 i = 0; i < gNumCoordBins * gNumCoordBins; i ++) {
		WRITECHECK(U32, interior->coordBin[i].binStart); //binStart
		WRITECHECK(U32, interior->coordBin[i].binCount); //binCount
	}
	WRITELOOPVAR(U16, interior->numCoordBinIndices, interior->coordBinIndex); //coordBinIndex
	WRITECHECK(U32, interior->coordBinMode); //coordBinMode
	WRITECHECK(ColorI, interior->baseAmbientColor); //baseAmbientColor
	WRITECHECK(ColorI, interior->alarmAmbientColor); //alarmAmbientColor
	/*
	 Static meshes (not included)
	 */
	WRITELOOPVAR(Point3F, interior->numTexNormals, interior->texNormal); //texNormal
	WRITELOOP(interior->numTexMatrices) { //numTexMatrices
		WRITECHECK(S32, interior->texMatrix[i].T); //T
		WRITECHECK(S32, interior->texMatrix[i].N); //N
		WRITECHECK(S32, interior->texMatrix[i].B); //B
	}
	WRITELOOPVAR(U32, interior->numTexMatIndices, interior->texMatIndex); //texMatIndex
	WRITECHECK(U32, interior->extendedLightMapData); //extendedLightMapData
	
	return true;
}

void interior_release(Interior *interior) {
	for (U32 i = 0; i < interior->numMaterials; i ++) {
		releaseString(interior->material[i]);

		if (interior->texture[i])
			texture_release(interior->texture[i]);
	}

	free(interior->normal);
	free(interior->plane);
	free(interior->point);
	free(interior->pointVisibility);
	free(interior->texGenEq);
	free(interior->BSPNode);
	free(interior->BSPSolidLeaf);
	free(interior->material);
	free(interior->texture);
	free(interior->index);
	free(interior->windingIndex);
	free(interior->zone);
	free(interior->zoneSurface);
	free(interior->zonePortalList);
	free(interior->portal);
	free(interior->surface);
	free(interior->normalLMapIndex);
	free(interior->alarmLMapIndex);
	free(interior->nullSurface);
	free(interior->lightMap);
	free(interior->solidLeafSurface);
	free(interior->animatedLight);
	free(interior->lightState);
	free(interior->stateData);
	free(interior->stateDataBuffer);
	free(interior->nameBufferCharacter);
	free(interior->convexHull);
	free(interior->convexHullEmitStringCharacter);
	free(interior->hullIndex);
	free(interior->hullPlaneIndex);
	free(interior->hullEmitStringIndex);
	free(interior->hullSurfaceIndex);
	free(interior->polyListPlaneIndex);
	free(interior->polyListPointIndex);
	free(interior->polyListStringCharacter);
	free(interior->coordBin);
	free(interior->coordBinIndex);
	free(interior->texNormal);
	free(interior->texMatrix);
	free(interior->texMatIndex);

	free(interior);
}

void interior_export_obj(Interior *interior, FILE *file) {
	for (U32 j = 0; j < interior->numPoints; j ++) {
		//Build triangles
		Point3F point = interior->point[j];
		fprintf(file, "v %g %g %g\n", -point.x, point.z, point.y);
	}

	for (U32 j = 0; j < interior->numNormals; j ++) {
		//Build triangles
		Point3F point = interior->normal[j];
		fprintf(file, "vn %g %g %g\n", -point.x, point.z, point.y);
	}

	fprintf(file, "\n");

	for (U32 surfaceNum = 0; surfaceNum < interior->numSurfaces; surfaceNum ++) {
		Surface surface = interior->surface[surfaceNum];

		U32 windingStart = surface.windingStart;
		U8 windingCount = surface.windingCount;

		fprintf(file, "f");

		//Triangle strips, in 0-1-2, 3-2-1, 2-3-4, 5-4-3 order
		for (U32 index = windingStart; index < windingStart + windingCount; index ++) {
			//Build triangles

			fprintf(file, " %d//%d", interior->index[index] + 1, interior->plane[surface.planeIndex].normalIndex + 1);
		}
		fprintf(file, "\n");
	}
}

U32 interior_ray_cast(Interior *interior, RayF ray) {
	ray = (RayF){point3F_convert_to_torque(ray.origin), point3F_convert_to_torque(ray.direction)};

	U32 closest = -1;
	F32 closestDistance = UINT32_MAX;

	printf("Ray: {(%f,%f,%f),(%f,%f,%f)}\n",ray.origin.x,ray.origin.y,ray.origin.z,ray.direction.x,ray.direction.y,ray.direction.z);

	for (U32 i = 0; i < interior->numSurfaces; i ++) {
		Surface surface = interior->surface[i];

		for (U32 j = 0; j < surface.windingCount - 2; j ++) {
			TriangleF triangle;
			triangle.point0 = interior->point[interior->index[surface.windingStart + j]];
			triangle.point1 = interior->point[interior->index[surface.windingStart + j + 1]];
			triangle.point2 = interior->point[interior->index[surface.windingStart + j + 2]];

			F32 distance = rayF_intersects_triangle(ray, triangle);
			if (distance > 0 && distance < closestDistance) {
				printf("Found closest triangle (surface %d, offset %d), distance is %f\n", i, j, distance);
				printf("Triangle: {(%f,%f,%f),(%f,%f,%f),(%f,%f,%f)}\n",triangle.point0.x,triangle.point0.y,triangle.point0.z,triangle.point1.x,triangle.point1.y,triangle.point1.z,triangle.point2.x,triangle.point2.y,triangle.point2.z);
				closestDistance = distance;
				closest = i;
			}
		}
	}

	return closest;
}
