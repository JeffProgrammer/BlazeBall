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

Interior::Interior(FILE *file, String *directory) {
	READTOVAR(interiorFileVersion, U32); //interiorFileVersion
	READTOVAR(detailLevel, U32); //detailLevel
	READTOVAR(minPixels, U32); //minPixels
	READTOVAR(boundingBox, BoxF); //boundingBox
	READTOVAR(boundingSphere, SphereF); //boundingSphere
	READTOVAR(hasAlarmState, U8); //hasAlarmState
	READTOVAR(numLightStateEntries, U32); //numLightStateEntries
	READLISTVAR(numNormals, normal, Point3F);
	READLISTVAR(numPlanes, plane, Plane);
	READLISTVAR(numPoints, point, Point3F);
	if (this->interiorFileVersion == 4) { //They exist in 0, 2, 3 but not 4
		//Probably defaulted to FF but uncertain
		numPointVisibilities = 0;
	} else {
		READLISTVAR(numPointVisibilities, pointVisibility, U8);
	}
	READLISTVAR(numTexGenEqs, texGenEq, TexGenEq);
	READLOOPVAR(numBSPNodes, BSPNode, ::BSPNode) {
		READTOVAR(BSPNode[i].planeIndex, U16); //planeIndex
		if (this->interiorFileVersion >= 14) {
			READTOVAR(BSPNode[i].frontIndex, U32); //frontIndex
			READTOVAR(BSPNode[i].backIndex, U32); //backIndex
		} else {
			READTOVAR(BSPNode[i].frontIndex, U16); //frontIndex
			READTOVAR(BSPNode[i].backIndex, U16); //backIndex
		}
	}
	READLISTVAR(numBSPSolidLeaves, BSPSolidLeaf, ::BSPSolidLeaf);
	//MaterialList
	READTOVAR(materialListVersion, U8); //version
	READLISTVAR(numMaterials, material, String);
	READLISTVAR2(numWindings, index, readnumWindings2, U32, U16);
	READLISTVAR(numWindingIndices, windingIndex, WindingIndex);
	if (this->interiorFileVersion >= 12) {
		READLISTVAR(numEdges, edge, Edge);
	} else {
		numEdges = 0;
	}
	READLOOPVAR(numZones, zone, Zone) {
		READTOVAR(zone[i].portalStart, U16); //portalStart
		READTOVAR(zone[i].portalCount, U16); //portalCount
		READTOVAR(zone[i].surfaceStart, U32); //surfaceStart
		READTOVAR(zone[i].surfaceCount, U32); //surfaceCount
		if (this->interiorFileVersion >= 12) {
			READTOVAR(zone[i].staticMeshStart, U32); //staticMeshStart
			READTOVAR(zone[i].staticMeshCount, U32); //staticMeshCount
			READTOVAR(zone[i].flags, U16); //flags
		} else {
			zone[i].staticMeshStart = 0;
			zone[i].staticMeshCount = 0;
			zone[i].flags = 0;
		}
	}
	READLISTVAR2(numZoneSurfaces, zoneSurface, 0, U16, U16);
	if (this->interiorFileVersion >= 12) {
		READLISTVAR(numZoneStaticMeshes, zoneStaticMesh, U32);
	} else {
		numZoneStaticMeshes = 0;
	}
	READLISTVAR(numZonePortalList, zonePortalList, U16);
	READLOOPVAR(numPortals, portal, Portal) {
		READTOVAR(portal[i].planeIndex, U16); //planeIndex
		READTOVAR(portal[i].triFanCount, U16); //triFanCount
		READTOVAR(portal[i].triFanStart, U32); //triFanStart
		READTOVAR(portal[i].zoneFront, U16); //zoneFront
		READTOVAR(portal[i].zoneBack, U16); //zoneBack
	}
	READLOOPVAR(numSurfaces, surface, Surface) {
		READTOVAR(surface[i].windingStart, U32); //windingStart
		if (this->interiorFileVersion >= 13) {
			READTOVAR(surface[i].windingCount, U32); //windingCount
		} else {
			READTOVAR(surface[i].windingCount, U8); //windingCount
		}
		//Fucking GarageGames. Sometimes the plane is | 0x8000 because WHY NOT
		READVAR(plane, S16); //planeIndex
		//Ugly hack
		surface[i].planeFlipped = (plane >> 15 != 0);
		plane &= ~0x8000;
		surface[i].planeIndex = plane;
		READTOVAR(surface[i].textureIndex, U16); //textureIndex
		READTOVAR(surface[i].texGenIndex, U32); //texGenIndex
		READTOVAR(surface[i].surfaceFlags, U8); //surfaceFlags
		READTOVAR(surface[i].fanMask, U32); //fanMask
		{ //LightMap
			READTOVAR(surface[i].lightMap.finalWord, U16); //finalWord
			READTOVAR(surface[i].lightMap.texGenXDistance, F32); //texGenXDistance
			READTOVAR(surface[i].lightMap.texGenYDistance, F32); //texGenYDistance
		}
		READTOVAR(surface[i].lightCount, U16); //lightCount
		READTOVAR(surface[i].lightStateInfoStart, U32); //lightStateInfoStart

		if (this->interiorFileVersion >= 13) {
			READTOVAR(surface[i].mapOffsetX, U32); //mapOffsetX
			READTOVAR(surface[i].mapOffsetY, U32); //mapOffsetY
			READTOVAR(surface[i].mapSizeX, U32); //mapSizeX
			READTOVAR(surface[i].mapSizeY, U32); //mapSizeY
		} else {
			READTOVAR(surface[i].mapOffsetX, U8); //mapOffsetX
			READTOVAR(surface[i].mapOffsetY, U8); //mapOffsetY
			READTOVAR(surface[i].mapSizeX, U8); //mapSizeX
			READTOVAR(surface[i].mapSizeY, U8); //mapSizeY
		}

		if (this->interiorFileVersion >= 1) {
			READ(U8); //unused
			if (this->interiorFileVersion <= 4) {
				READ(U32); //Extra bytes used for some unknown purpose, seen in versions 2, 3, 4
			}
		}
	}
	if (this->interiorFileVersion >= 2 && this->interiorFileVersion <= 4) {
		//Extra data that I've seen in MBU interiors (v2, 3, 4)
		READLOOP(numIndicesOfSomeSort) {
			//Potentially brush data for constructor... I don't know

			//I really don't know what these are, only their size
			READ(U32);
			READ(U32);
			READ(U32);
			READ(U32);

			//Two extra that are missing in v2
			if (this->interiorFileVersion >= 3) {
				READ(U32);
				READ(U32);
			}
		}
		//v4 has some extra points and indices, no clue what these are either
		if (this->interiorFileVersion == 4) {
			//May be brush points, normals, no clue
			READLIST(numPointsOfSomeKind, Point3F); //Not sure, normals of some sort
			//Looks like indcies of some sort, can't seem to make them out though

			//Unlike anywhere else, these actually take the param into account.
			// If it's read2 and param == 0, then they use U8s, if param == 1, they use U16s
			// Not really sure why, haven't seen this anywhere else.
			READLIST2(numSomethingElses, (readnumSomethingElses2 && readnumSomethingElsesparam == 0), U16, U8);
		}
	}
	READLISTVAR(numNormalLMapIndices, normalLMapIndex, U8);
	if (this->interiorFileVersion == 4) { //Found in 0, 2, 3, and TGE (14)
		numAlarmLMapIndices = 0;
	} else {
		READLISTVAR(numAlarmLMapIndices, alarmLMapIndex, U8);
	}
	READLOOPVAR(numNullSurfaces, nullSurface, NullSurface) {
		READTOVAR(nullSurface[i].windingStart, U32); //windingStart
		READTOVAR(nullSurface[i].planeIndex, U16); //planeIndex
		READTOVAR(nullSurface[i].surfaceFlags, U8); //surfaceFlags
		READTOVAR(nullSurface[i].windingCount, U8); //windingCount
	}
	if (this->interiorFileVersion == 4) { //Also found in 0, 2, 3, 14
		numLightMaps = 0;
	} else {
		READLOOPVAR(numLightMaps, lightMap, LightMap) {
			READTOVAR(lightMap[i].lightMap, PNG); //lightMap
			if (this->interiorFileVersion >= 2) {
				READTOVAR(lightMap[i].lightDirMap, PNG); //lightDirMap
			}
			READTOVAR(lightMap[i].keepLightMap, U8); //keepLightMap
		}
	}
	READLISTVAR2(numSolidLeafSurfaces, solidLeafSurface, (readnumSolidLeafSurfaces2), U32, U16);
	READLOOPVAR(numAnimatedLights, animatedLight, AnimatedLight) {
		READTOVAR(animatedLight[i].nameIndex, U32); //nameIndex
		READTOVAR(animatedLight[i].stateIndex, U32); //stateIndex
		READTOVAR(animatedLight[i].stateCount, U16); //stateCount
		READTOVAR(animatedLight[i].flags, U16); //flags
		READTOVAR(animatedLight[i].duration, U32); //duration
	}
	READLOOPVAR(numLightStates, lightState, LightState) {
		READTOVAR(lightState[i].red, U8); //red
		READTOVAR(lightState[i].green, U8); //green
		READTOVAR(lightState[i].blue, U8); //blue
		READTOVAR(lightState[i].activeTime, U32); //activeTime
		READTOVAR(lightState[i].dataIndex, U32); //dataIndex
		READTOVAR(lightState[i].dataCount, U16); //dataCount
	}
	if (this->interiorFileVersion == 4) { //Yet more things found in 0, 2, 3, 14
		numStateDatas = 0;
		numStateDataBuffers = 0;
		flags = 0;
		numNameBuffers = 0;
		numSubObjects = 0;
	} else {
		READLOOPVAR(numStateDatas, stateData, StateData) {
			READTOVAR(stateData[i].surfaceIndex, U32); //surfaceIndex
			READTOVAR(stateData[i].mapIndex, U32); //mapIndex
			READTOVAR(stateData[i].lightStateIndex, U16); //lightStateIndex
		}
		READLISTVAR(numStateDataBuffers, stateDataBuffer, U8);
		READTOVAR(flags, U32); //flags
		READLISTVAR(numNameBuffers, nameBufferCharacter, U8);

		READLOOP(numSubObjects) {
			//NFC
		}
	}
	READLOOPVAR(numConvexHulls, convexHull, ConvexHull) {
		READTOVAR(convexHull[i].hullStart, U32); //hullStart
		READTOVAR(convexHull[i].hullCount, U16); //hullCount
		READTOVAR(convexHull[i].minX, F32); //minX
		READTOVAR(convexHull[i].maxX, F32); //maxX
		READTOVAR(convexHull[i].minY, F32); //minY
		READTOVAR(convexHull[i].maxY, F32); //maxY
		READTOVAR(convexHull[i].minZ, F32); //minZ
		READTOVAR(convexHull[i].maxZ, F32); //maxZ
		READTOVAR(convexHull[i].surfaceStart, U32); //surfaceStart
		READTOVAR(convexHull[i].surfaceCount, U16); //surfaceCount
		READTOVAR(convexHull[i].planeStart, U32); //planeStart
		READTOVAR(convexHull[i].polyListPlaneStart, U32); //polyListPlaneStart
		READTOVAR(convexHull[i].polyListPointStart, U32); //polyListPointStart
		READTOVAR(convexHull[i].polyListStringStart, U32); //polyListStringStart

		if (this->interiorFileVersion >= 12) {
			READTOVAR(convexHull[i].staticMesh, U8); //staticMesh
		} else {
			convexHull[i].staticMesh = 0;
		}
	}
	READLISTVAR(numConvexHullEmitStrings, convexHullEmitStringCharacter, U8);

	//-------------------------------------------------------------------------
	// Lots of index lists here that have U16 or U32 versions based on loop2.
	// The actual bytes of the interior have 0x80s at the ends (negative bit)
	// which seems to specify that these take a smaller type. They managed to
	// save ~50KB/interior, but was it worth the pain?
	//
	// Also fun fact: the U16 lists have literally no reason for the 0x80, as
	// they're already using U16s. However, GG still puts them in. What the
	// fuck, GarageGames?
	//-------------------------------------------------------------------------

	READLISTVAR2(numHullIndices, hullIndex, (readnumHullIndices2), U32, U16);
	READLISTVAR2(numHullPlaneIndices, hullPlaneIndex, 0, U16, U16);
	READLISTVAR2(numHullEmitStringIndices, hullEmitStringIndex, (readnumHullEmitStringIndices2), U32, U16);
	READLISTVAR2(numHullSurfaceIndices, hullSurfaceIndex, (readnumHullSurfaceIndices2), U32, U16);
	READLISTVAR2(numPolyListPlanes, polyListPlaneIndex, 0, U16, U16);
	READLISTVAR2(numPolyListPoints, polyListPointIndex, (readnumPolyListPoints2), U32, U16);
	//Not sure if this should be a READLISTVAR2, but I haven't seen any evidence
	// of needing that for U8 lists.
	READLISTVAR(numPolyListStrings, polyListStringCharacter, U8);

	coordBin = new CoordBin[gNumCoordBins * gNumCoordBins];
	for (U32 i = 0; i < gNumCoordBins * gNumCoordBins; i ++) {
		READTOVAR(coordBin[i].binStart, U32); //binStart
		READTOVAR(coordBin[i].binCount, U32); //binCount
	}

	READLISTVAR2(numCoordBinIndices, coordBinIndex, 0, U16, U16);
	READTOVAR(coordBinMode, U32); //coordBinMode
	if (this->interiorFileVersion == 4) { //All of this is missing in v4 as well. Saves no space.
		baseAmbientColor = ColorI(0, 0, 0, 255);
		alarmAmbientColor = ColorI(0, 0, 0, 255);
		numTexNormals = 0;
		numTexMatrices = 0;
		numTexMatIndices = 0;
		extendedLightMapData = 0;
		lightMapBorderSize = 0;
	} else {
		READTOVAR(baseAmbientColor, ColorI); //baseAmbientColor
		READTOVAR(alarmAmbientColor, ColorI); //alarmAmbientColor
		/*
		 There's a long list of static meshes here, but I'm too lazy to add it just to comment it out. Oh and I'd have to implement Point2I / Point2F. See DIF_MB_SPEC.rtf if you want to implement it.
		 */
		READLISTVAR(numTexNormals, texNormal, Point3F);
		READLOOPVAR(numTexMatrices, texMatrix, TexMatrix) {
			READTOVAR(texMatrix[i].T, S32); //T
			READTOVAR(texMatrix[i].N, S32); //N
			READTOVAR(texMatrix[i].B, S32); //B
		}
		READLISTVAR(numTexMatIndices, texMatIndex, U32);
		READTOVAR(extendedLightMapData, U32);
		if (extendedLightMapData) { //extendedLightMapData
			READTOVAR(lightMapBorderSize, U32); //lightMapBorderSize
			READ(U32); //dummy
		} else {
			lightMapBorderSize = 0;
		}
	}

	//Allocate all textures for the interior
	if (numMaterials) {
		texture = (Texture **)new Texture*[numMaterials];
		for (U32 i = 0; i < numMaterials; i ++) {
			String *material = new String(this->material[i].length + 1);
			strcpy((char *)material->data, (const char *)this->material[i]);
			//Chop off any paths from the material. Constructor likes to save albums in the materials
			// and it royally breaks this program.
			if (strstr((const char *)material->data, "/")) {
				//Hacky but effective method
				strcpy((char *)material->data, strstr((const char *)material->data, "/") + 1);
			}

			BitmapType type = BitmapTypePNG;

			//For some reason these two like to become the same.
			String *base = new String(directory);

			//Allocate enough space in each of these so we can work comfortably
			U32 pathlen = (U32)(base->length + material->length + 1);
			String *imageFile = new String(pathlen + 5);

			do {
				//Init imageFile to base/file.png
				pathlen = sprintf((char *)imageFile->data, "%s/%s.png", (char *)base->data, (char *)material->data);

				type = BitmapTypePNG;

				//If we can't find the PNG, try for JPEG
				//TODO: BMP Support?
				if (!io->isfile(imageFile)) {
					//Swap the last 3 chars with jpg
					imageFile->data[pathlen - 3] = 'j';
					imageFile->data[pathlen - 2] = 'p';
					imageFile->data[pathlen - 1] = 'g';
					type = BitmapTypeJPEG;
				}
				//Can't recurse any further
				if (!strrchr((const char *)base->data, '/'))
					break;

				//If we still can't find it, recurse (hacky but effective method)
				if (!io->isfile(imageFile))
					*strrchr((const char *)base->data, '/') = 0;
			} while (!io->isfile(imageFile) && strcmp((const char *)base->data, ""));

			//If we can't find it, just chuck the lot and keep going.
			if (!io->isfile(imageFile)) {
				fprintf(stderr, "Error in reading bitmap: %s Bitmap not found.\n", (char *)material->data);
				texture[i] = NULL;
				free(base);
				delete [] imageFile;
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

			if (!readFn(*imageFile, &bitmap, &dims)) {
				fprintf(stderr, "Error in reading bitmap: %s Other error\n", (char *)imageFile->data);
				texture[i] = NULL;

				free(bitmap);
				free(base);
				free(imageFile);
				continue;
			}

			//Create a texture from the bitmap (copies bitmap)
			Texture *texture = new Texture(bitmap, dims);
			this->texture[i] = texture;

			//Clean up bitmap (copied above, this is safe)
			free(bitmap);
			free(base);
			free(imageFile);
		}
	}
}

bool Interior::write(FILE *file) {
	//We can only write version 0 maps at the moment.

	WRITECHECK(U32, 0); //interiorFileVersion
	WRITECHECK(U32, detailLevel); //detailLevel
	WRITECHECK(U32, minPixels); //minPixels
	WRITECHECK(BoxF, boundingBox); //boundingBox
	WRITECHECK(SphereF, boundingSphere); //boundingSphere
	WRITECHECK(U8, hasAlarmState); //hasAlarmState
	WRITECHECK(U32, numLightStateEntries); //numLightStateEntries
	WRITELOOPVAR(Point3F, numNormals, normal); //normal
	WRITELOOP(numPlanes) { //numPlanes
		WRITECHECK(U16, plane[i].normalIndex); //normalIndex
		WRITECHECK(F32, plane[i].planeDistance); //planeDistance
	}
	WRITELOOPVAR(Point3F, numPoints, point); //point
	WRITELOOPVAR(U8, numPointVisibilities, pointVisibility); //pointVisibility
	WRITELOOP(numTexGenEqs) { //numTexGenEqs
		WRITECHECK(PlaneF, texGenEq[i].planeX); //planeX
		WRITECHECK(PlaneF, texGenEq[i].planeY); //planeY
	}
	WRITELOOP(numBSPNodes) { //numBSPNodes
		WRITECHECK(U16, BSPNode[i].planeIndex); //planeIndex
		WRITECHECK(U16, BSPNode[i].frontIndex); //frontIndex
		WRITECHECK(U16, BSPNode[i].backIndex); //backIndex
	}
	WRITELOOP(numBSPSolidLeaves) { //numBSPSolidLeaves
		WRITECHECK(U32, BSPSolidLeaf[i].surfaceIndex); //surfaceIndex
		WRITECHECK(U16, BSPSolidLeaf[i].surfaceCount); //surfaceCount
	}
	WRITECHECK(U8, materialListVersion); //materialListVersion
	WRITELOOP(numMaterials) { //numMaterials
		WRITE(String, material[i]); //material
	}
	WRITELOOPVAR(U32, numWindings, index); //index
	WRITELOOP(numWindingIndices) { //numWindingIndices
		WRITECHECK(U32, windingIndex[i].windingStart); //windingStart
		WRITECHECK(U32, windingIndex[i].windingCount); //windingCount
	}
	WRITELOOP(numZones) { //numZones
		WRITECHECK(U16, zone[i].portalStart); //portalStart
		WRITECHECK(U16, zone[i].portalCount); //portalCount
		WRITECHECK(U32, zone[i].surfaceStart); //surfaceStart
		WRITECHECK(U32, zone[i].surfaceCount); //surfaceCount
	}
	WRITELOOPVAR(U16, numZoneSurfaces, zoneSurface); //zoneSurface
	WRITELOOPVAR(U16, numZonePortalList, zonePortalList); //zonePortalList
	WRITELOOP(numPortals) { //numPortals
		WRITECHECK(U16, portal[i].planeIndex); //planeIndex
		WRITECHECK(U16, portal[i].triFanCount); //triFanCount
		WRITECHECK(U32, portal[i].triFanStart); //triFanStart
		WRITECHECK(U16, portal[i].zoneFront); //zoneFront
		WRITECHECK(U16, portal[i].zoneBack); //zoneBack
	}
	WRITELOOP(numSurfaces) { //numSurfaces
		WRITECHECK(U32, surface[i].windingStart); //windingStart
		WRITECHECK(U8, surface[i].windingCount); //windingCount
		U16 index = surface[i].planeIndex;
		if (surface[i].planeFlipped)
			index |= 0x8000;
		WRITECHECK(U16, index); //planeIndex
		WRITECHECK(U16, surface[i].textureIndex); //textureIndex
		WRITECHECK(U32, surface[i].texGenIndex); //texGenIndex
		WRITECHECK(U8, surface[i].surfaceFlags); //surfaceFlags
		WRITECHECK(U32, surface[i].fanMask); //fanMask
		WRITECHECK(U16, surface[i].lightMap.finalWord); //finalWord
		WRITECHECK(F32, surface[i].lightMap.texGenXDistance); //texGenXDistance
		WRITECHECK(F32, surface[i].lightMap.texGenYDistance); //texGenYDistance
		WRITECHECK(U16, surface[i].lightCount); //lightCount
		WRITECHECK(U32, surface[i].lightStateInfoStart); //lightStateInfoStart
		WRITECHECK(U8, surface[i].mapOffsetX); //mapOffsetX
		WRITECHECK(U8, surface[i].mapOffsetY); //mapOffsetY
		WRITECHECK(U8, surface[i].mapSizeX); //mapSizeX
		WRITECHECK(U8, surface[i].mapSizeY); //mapSizeY
	}
	WRITELOOPVAR(U8, numNormalLMapIndices, normalLMapIndex); //normalLMapIndex
	WRITELOOPVAR(U8, numAlarmLMapIndices, alarmLMapIndex); //alarmLMapIndex
	WRITELOOP(numNullSurfaces) { //numNullSurfaces
		WRITECHECK(U32, nullSurface[i].windingStart); //windingStart
		WRITECHECK(U16, nullSurface[i].planeIndex); //planeIndex
		WRITECHECK(U8, nullSurface[i].surfaceFlags); //surfaceFlags
		WRITECHECK(U8, nullSurface[i].windingCount); //windingCount
	}
	WRITELOOP(numLightMaps) { //numLightMaps
		WRITE(PNG, lightMap[i].lightMap); //lightMap
		WRITECHECK(U8, lightMap[i].keepLightMap); //keepLightMap
	}
	WRITELOOPVAR(U32, numSolidLeafSurfaces, solidLeafSurface); //solidLeafSurface
	WRITELOOP(numAnimatedLights) { //numAnimatedLights
		WRITECHECK(U32, animatedLight[i].nameIndex); //nameIndex
		WRITECHECK(U32, animatedLight[i].stateIndex); //stateIndex
		WRITECHECK(U16, animatedLight[i].stateCount); //stateCount
		WRITECHECK(U16, animatedLight[i].flags); //flags
		WRITECHECK(U32, animatedLight[i].duration); //duration
	}
	WRITELOOP(numLightStates) { //numLightStates
		WRITECHECK(U8, lightState[i].red); //red
		WRITECHECK(U8, lightState[i].green); //green
		WRITECHECK(U8, lightState[i].blue); //blue
		WRITECHECK(U32, lightState[i].activeTime); //activeTime
		WRITECHECK(U32, lightState[i].dataIndex); //dataIndex
		WRITECHECK(U16, lightState[i].dataCount); //dataCount
	}
	WRITELOOP(numStateDatas) { //numStateDatas
		WRITECHECK(U32, stateData[i].surfaceIndex); //surfaceIndex
		WRITECHECK(U32, stateData[i].mapIndex); //mapIndex
		WRITECHECK(U16, stateData[i].lightStateIndex); //lightStateIndex
	}
	WRITELOOPVAR(U32, numStateDataBuffers, stateDataBuffer); //stateDataBuffer
	WRITECHECK(U32, flags); //flags
	WRITELOOPVAR(S8, numNameBuffers, nameBufferCharacter); //nameBufferCharacter
	WRITELOOP(numSubObjects) { //numSubObjects

	}
	WRITELOOP(numConvexHulls) { //numConvexHulls
		WRITECHECK(U32, convexHull[i].hullStart); //hullStart
		WRITECHECK(U16, convexHull[i].hullCount); //hullCount
		WRITECHECK(F32, convexHull[i].minX); //minX
		WRITECHECK(F32, convexHull[i].maxX); //maxX
		WRITECHECK(F32, convexHull[i].minY); //minY
		WRITECHECK(F32, convexHull[i].maxY); //maxY
		WRITECHECK(F32, convexHull[i].minZ); //minZ
		WRITECHECK(F32, convexHull[i].maxZ); //maxZ
		WRITECHECK(U32, convexHull[i].surfaceStart); //surfaceStart
		WRITECHECK(U16, convexHull[i].surfaceCount); //surfaceCount
		WRITECHECK(U32, convexHull[i].planeStart); //planeStart
		WRITECHECK(U32, convexHull[i].polyListPlaneStart); //polyListPlaneStart
		WRITECHECK(U32, convexHull[i].polyListPointStart); //polyListPointStart
		WRITECHECK(U32, convexHull[i].polyListStringStart); //polyListStringStart
	}
	WRITELOOPVAR(U8, numConvexHullEmitStrings, convexHullEmitStringCharacter); //convexHullEmitStringCharacter
	WRITELOOPVAR(U32, numHullIndices, hullIndex); //hullIndex
	WRITELOOPVAR(U16, numHullPlaneIndices, hullPlaneIndex); //hullPlaneIndex
	WRITELOOPVAR(U32, numHullEmitStringIndices, hullEmitStringIndex); //hullEmitStringIndex
	WRITELOOPVAR(U32, numHullSurfaceIndices, hullSurfaceIndex); //hullSurfaceIndex
	WRITELOOPVAR(U16, numPolyListPlanes, polyListPlaneIndex); //polyListPlaneIndex
	WRITELOOPVAR(U32, numPolyListPoints, polyListPointIndex); //polyListPointIndex
	WRITELOOPVAR(U8, numPolyListStrings, polyListStringCharacter); //polyListStringCharacter
	for (U32 i = 0; i < gNumCoordBins * gNumCoordBins; i ++) {
		WRITECHECK(U32, coordBin[i].binStart); //binStart
		WRITECHECK(U32, coordBin[i].binCount); //binCount
	}
	WRITELOOPVAR(U16, numCoordBinIndices, coordBinIndex); //coordBinIndex
	WRITECHECK(U32, coordBinMode); //coordBinMode
	WRITECHECK(ColorI, baseAmbientColor); //baseAmbientColor
	WRITECHECK(ColorI, alarmAmbientColor); //alarmAmbientColor
	/*
	 Static meshes (not included)
	 */
	WRITELOOPVAR(Point3F, numTexNormals, texNormal); //texNormal
	WRITELOOP(numTexMatrices) { //numTexMatrices
		WRITECHECK(S32, texMatrix[i].T); //T
		WRITECHECK(S32, texMatrix[i].N); //N
		WRITECHECK(S32, texMatrix[i].B); //B
	}
	WRITELOOPVAR(U32, numTexMatIndices, texMatIndex); //texMatIndex
	WRITECHECK(U32, extendedLightMapData); //extendedLightMapData
	
	return true;
}

Interior::~Interior() {
	for (U32 i = 0; i < numMaterials; i ++) {
		releaseString(material[i]);

		if (texture[i])
			delete texture[i];
	}

	free(normal);
	free(plane);
	free(point);
	free(pointVisibility);
	free(texGenEq);
	free(BSPNode);
	free(BSPSolidLeaf);
	free(material);
	free(texture);
	free(index);
	free(windingIndex);
	free(zone);
	free(zoneSurface);
	free(zonePortalList);
	free(portal);
	free(surface);
	free(normalLMapIndex);
	free(alarmLMapIndex);
	free(nullSurface);
	free(lightMap);
	free(solidLeafSurface);
	free(animatedLight);
	free(lightState);
	free(stateData);
	free(stateDataBuffer);
	free(nameBufferCharacter);
	free(convexHull);
	free(convexHullEmitStringCharacter);
	free(hullIndex);
	free(hullPlaneIndex);
	free(hullEmitStringIndex);
	free(hullSurfaceIndex);
	free(polyListPlaneIndex);
	free(polyListPointIndex);
	free(polyListStringCharacter);
	free(coordBin);
	free(coordBinIndex);
	free(texNormal);
	free(texMatrix);
	free(texMatIndex);
}

void Interior::exportObj(FILE *file) {
	for (U32 j = 0; j < numPoints; j ++) {
		//Build triangles
		Point3F point = this->point[j];
		fprintf(file, "v %g %g %g\n", -point.x, point.z, point.y);
	}

	for (U32 j = 0; j < numNormals; j ++) {
		//Build triangles
		Point3F point = normal[j];
		fprintf(file, "vn %g %g %g\n", -point.x, point.z, point.y);
	}

	fprintf(file, "\n");

	for (U32 surfaceNum = 0; surfaceNum < numSurfaces; surfaceNum ++) {
		Surface surface = this->surface[surfaceNum];

		U32 windingStart = surface.windingStart;
		U8 windingCount = surface.windingCount;

		fprintf(file, "f");

		//Triangle strips, in 0-1-2, 3-2-1, 2-3-4, 5-4-3 order
		for (U32 index = windingStart; index < windingStart + windingCount; index ++) {
			//Build triangles

			fprintf(file, " %d//%d", this->index[index] + 1, plane[surface.planeIndex].normalIndex + 1);
		}
		fprintf(file, "\n");
	}
}

U32 Interior::rayCast(RayF ray) {
//	ray = RayF(ray.origin.convert(), ray.direction.convert());

	U32 closest = -1;
	F32 closestDistance = UINT32_MAX;

	printf("Ray: {(%f,%f,%f),(%f,%f,%f)}\n",ray.origin.x,ray.origin.y,ray.origin.z,ray.direction.x,ray.direction.y,ray.direction.z);

	for (U32 i = 0; i < numSurfaces; i ++) {
		Surface surface = this->surface[i];

		for (U32 j = 0; j < surface.windingCount - 2; j ++) {
			TriangleF triangle;
			triangle.point0 = point[index[surface.windingStart + j]];
			triangle.point1 = point[index[surface.windingStart + j + 1]];
			triangle.point2 = point[index[surface.windingStart + j + 2]];

			F32 distance = ray.distance(triangle);
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
//----------------------------------------------------------------------------

bool Plane::read(FILE *file) {
	READTOVAR(normalIndex, U16); //normalIndex
	READTOVAR(planeDistance, F32); //planeDistance
	return true;
}

bool Plane::write(FILE *file) {
	WRITECHECK(U16, normalIndex); //normalIndex
	WRITECHECK(F32, planeDistance); //planeDistance
	return true;
}

bool TexGenEq::read(FILE *file) {
	READTOVAR(planeX, PlaneF); //planeX
	READTOVAR(planeY, PlaneF); //planeY
	return true;
}

bool TexGenEq::write(FILE *file) {
	WRITECHECK(PlaneF, planeX); //planeX
	WRITECHECK(PlaneF, planeY); //planeY
	return true;
}

bool BSPSolidLeaf::read(FILE *file) {
	READTOVAR(surfaceIndex, U32); //surfaceIndex
	READTOVAR(surfaceCount, U16); //surfaceCount
	return true;
}

bool BSPSolidLeaf::write(FILE *file) {
	WRITECHECK(U32, surfaceIndex); //surfaceIndex
	WRITECHECK(U16, surfaceCount); //surfaceCount
	return true;
}

bool WindingIndex::read(FILE *file) {
	READTOVAR(windingStart, U32); //windingStart
	READTOVAR(windingCount, U32); //windingCount
	return true;
}

bool WindingIndex::write(FILE *file) {
	WRITECHECK(U32, windingStart); //windingStart
	WRITECHECK(U32, windingCount); //windingCount
	return true;
}

bool Edge::read(FILE *file) {
	READTOVAR(pointIndex0, S32); //pointIndex0
	READTOVAR(pointIndex1, S32); //pointIndex1
	READTOVAR(surfaceIndex0, S32); //surfaceIndex0
	READTOVAR(surfaceIndex1, S32); //surfaceIndex1
	return true;
}

bool Edge::write(FILE *file) {
	WRITECHECK(S32, pointIndex0); //pointIndex0
	WRITECHECK(S32, pointIndex1); //pointIndex1
	WRITECHECK(S32, surfaceIndex0); //surfaceIndex0
	WRITECHECK(S32, surfaceIndex1); //surfaceIndex1
	return true;
}