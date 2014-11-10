//------------------------------------------------------------------------------
// Copyright (C) 2014 Glenn Smith
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//------------------------------------------------------------------------------

//Warning: This is the scary file.

#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "interior.h"

Interior *interior_read_file(FILE *file) {
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
		READ(PNG); //lightMap
		/*
		 Not in MB
		 READTOVAR(interior->lightDirMap[i], PNG); //lightDirMap
		 */
		READ(U8); //keepLightMap
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
	READTOVAR(interior->baseAmbientColor, ColorF); //baseAmbientColor
	READTOVAR(interior->alarmAmbientColor, ColorF); //alarmAmbientColor
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

	return interior;
}

void interior_release(Interior *interior) {
	for (U32 i = 0; i < interior->numMaterials; i ++) {
		releaseString(interior->material[i]);
	}

	free(interior->normal);
	free(interior->plane);
	free(interior->point);
	free(interior->pointVisibility);
	free(interior->texGenEq);
	free(interior->BSPNode);
	free(interior->BSPSolidLeaf);
	free(interior->material);
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
