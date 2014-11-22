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

#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "vehicleCollision.h"

VehicleCollision *vehicleCollision_read_file(FILE *file) {
	VehicleCollision *vehicleCollision = malloc(sizeof(VehicleCollision));

	READTOVAR(vehicleCollision->vehicleCollisionFileVersion, U32); //vehicleCollisionFileVersion
	READLOOPVAR(vehicleCollision->numVehicleConvexHulls, vehicleCollision->vehicleConvexHull, VehicleConvexHull) {
		READTOVAR(vehicleCollision->vehicleConvexHull[i].hullStart, U32); //hullStart
		READTOVAR(vehicleCollision->vehicleConvexHull[i].hullCount, U16); //hullCount
		READTOVAR(vehicleCollision->vehicleConvexHull[i].minX, F32); //minX
		READTOVAR(vehicleCollision->vehicleConvexHull[i].maxX, F32); //maxX
		READTOVAR(vehicleCollision->vehicleConvexHull[i].minY, F32); //minY
		READTOVAR(vehicleCollision->vehicleConvexHull[i].maxY, F32); //maxY
		READTOVAR(vehicleCollision->vehicleConvexHull[i].minZ, F32); //minZ
		READTOVAR(vehicleCollision->vehicleConvexHull[i].maxZ, F32); //maxZ
		READTOVAR(vehicleCollision->vehicleConvexHull[i].surfaceStart, U32); //surfaceStart
		READTOVAR(vehicleCollision->vehicleConvexHull[i].surfaceCount, U16); //surfaceCount
		READTOVAR(vehicleCollision->vehicleConvexHull[i].planeStart, U32); //planeStart
		READTOVAR(vehicleCollision->vehicleConvexHull[i].polyListPlaneStart, U32); //polyListPlaneStart
		READTOVAR(vehicleCollision->vehicleConvexHull[i].polyListPointStart, U32); //polyListPointStart
		READTOVAR(vehicleCollision->vehicleConvexHull[i].polyListStringStart, U32); //polyListStringStart
	}
	READLOOPVAR(vehicleCollision->numVehicleConvexHullEmitStrings, vehicleCollision->vehicleConvexHullEmitStringCharacter, U8) {
		READTOVAR(vehicleCollision->vehicleConvexHullEmitStringCharacter[i], U8); //vehicleConvexHullEmitStringCharacter
	}
	READLOOPVAR(vehicleCollision->numVehicleHullIndices, vehicleCollision->vehicleHullIndex, U32) {
		READTOVAR(vehicleCollision->vehicleHullIndex[i], U32); //vehicleHullIndex
	}
	READLOOPVAR(vehicleCollision->numVehicleHullPlaneIndices, vehicleCollision->vehicleHullPlaneIndex, U16) {
		READTOVAR(vehicleCollision->vehicleHullPlaneIndex[i], U16); //vehicleHullPlaneIndex
	}
	READLOOPVAR(vehicleCollision->numVehicleHullEmitStringIndices, vehicleCollision->vehicleHullEmitStringIndex, U32) {
		READTOVAR(vehicleCollision->vehicleHullEmitStringIndex[i], U32); //vehicleHullEmitStringIndex
	}
	READLOOPVAR(vehicleCollision->numVehicleHullSurfaceIndices, vehicleCollision->vehicleHullSurfaceIndex, U32) {
		READTOVAR(vehicleCollision->vehicleHullSurfaceIndex[i], U32); //vehicleHullSurfaceIndex
	}
	READLOOPVAR(vehicleCollision->numVehiclePolyListPlanes, vehicleCollision->vehiclePolyListPlaneIndex, U16) {
		READTOVAR(vehicleCollision->vehiclePolyListPlaneIndex[i], U16); //vehiclePolyListPlaneIndex
	}
	READLOOPVAR(vehicleCollision->numVehiclePolyListPoints, vehicleCollision->vehiclePolyListPointIndex, U32) {
		READTOVAR(vehicleCollision->vehiclePolyListPointIndex[i], U32); //vehiclePolyListPointIndex
	}
	READLOOPVAR(vehicleCollision->numVehiclePolyListStrings, vehicleCollision->vehiclePolyListStringCharacter, U8) {
		READTOVAR(vehicleCollision->vehiclePolyListStringCharacter[i], U8); //vehiclePolyListStringCharacter
	}
	READLOOPVAR(vehicleCollision->numVehicleNullSurfaces, vehicleCollision->vehicleNullSurface, VehicleNullSurface) {
		READTOVAR(vehicleCollision->vehicleNullSurface[i].windingStart, U32); //windingStart
		READTOVAR(vehicleCollision->vehicleNullSurface[i].planeIndex, U16); //planeIndex
		READTOVAR(vehicleCollision->vehicleNullSurface[i].surfaceFlags, U8); //surfaceFlags
		READTOVAR(vehicleCollision->vehicleNullSurface[i].windingCount, U32); //windingCount
	}

	return vehicleCollision;
}

bool vehicleCollision_write_file(FILE *file, VehicleCollision *vehicleCollision) {
	WRITECHECK(U32, vehicleCollision->vehicleCollisionFileVersion);
	WRITELOOP(vehicleCollision->numVehicleConvexHulls) {
		WRITECHECK(U32, vehicleCollision->vehicleConvexHull[i].hullStart); //hullStart
		WRITECHECK(U16, vehicleCollision->vehicleConvexHull[i].hullCount); //hullCount
		WRITECHECK(F32, vehicleCollision->vehicleConvexHull[i].minX); //minX
		WRITECHECK(F32, vehicleCollision->vehicleConvexHull[i].maxX); //maxX
		WRITECHECK(F32, vehicleCollision->vehicleConvexHull[i].minY); //minY
		WRITECHECK(F32, vehicleCollision->vehicleConvexHull[i].maxY); //maxY
		WRITECHECK(F32, vehicleCollision->vehicleConvexHull[i].minZ); //minZ
		WRITECHECK(F32, vehicleCollision->vehicleConvexHull[i].maxZ); //maxZ
		WRITECHECK(U32, vehicleCollision->vehicleConvexHull[i].surfaceStart); //surfaceStart
		WRITECHECK(U16, vehicleCollision->vehicleConvexHull[i].surfaceCount); //surfaceCount
		WRITECHECK(U32, vehicleCollision->vehicleConvexHull[i].planeStart); //planeStart
		WRITECHECK(U32, vehicleCollision->vehicleConvexHull[i].polyListPlaneStart); //polyListPlaneStart
		WRITECHECK(U32, vehicleCollision->vehicleConvexHull[i].polyListPointStart); //polyListPointStart
		WRITECHECK(U32, vehicleCollision->vehicleConvexHull[i].polyListStringStart); //polyListStringStart
	}
	WRITELOOPVAR(U8, vehicleCollision->numVehicleConvexHullEmitStrings, vehicleCollision->vehicleConvexHullEmitStringCharacter); //vehicleConvexHullEmitStringCharacter
	WRITELOOPVAR(U32, vehicleCollision->numVehicleHullIndices, vehicleCollision->vehicleHullIndex); //vehicleHullIndex
	WRITELOOPVAR(U16, vehicleCollision->numVehicleHullPlaneIndices, vehicleCollision->vehicleHullPlaneIndex); //vehicleHullPlaneIndex
	WRITELOOPVAR(U32, vehicleCollision->numVehicleHullEmitStringIndices, vehicleCollision->vehicleHullEmitStringIndex); //vehicleHullEmitStringIndex
	WRITELOOPVAR(U32, vehicleCollision->numVehicleHullSurfaceIndices, vehicleCollision->vehicleHullSurfaceIndex); //vehicleHullSurfaceIndex
	WRITELOOPVAR(U16, vehicleCollision->numVehiclePolyListPlanes, vehicleCollision->vehiclePolyListPlaneIndex); //vehiclePolyListPlaneIndex
	WRITELOOPVAR(U32, vehicleCollision->numVehiclePolyListPoints, vehicleCollision->vehiclePolyListPointIndex); //vehiclePolyListPointIndex
	WRITELOOPVAR(U8, vehicleCollision->numVehiclePolyListStrings, vehicleCollision->vehiclePolyListStringCharacter); //vehiclePolyListStringCharacter
	WRITELOOP(vehicleCollision->numVehicleNullSurfaces) {
		WRITECHECK(U32, vehicleCollision->vehicleNullSurface[i].windingStart); //windingStart
		WRITECHECK(U16, vehicleCollision->vehicleNullSurface[i].planeIndex); //planeIndex
		WRITECHECK(U8, vehicleCollision->vehicleNullSurface[i].surfaceFlags); //surfaceFlags
		WRITECHECK(U32, vehicleCollision->vehicleNullSurface[i].windingCount); //windingCount
	}

	return true;
}

void vehicleCollision_release(VehicleCollision *vehicleCollision) {
	free(vehicleCollision->vehicleConvexHull);
	free(vehicleCollision->vehicleConvexHullEmitStringCharacter);
	free(vehicleCollision->vehicleHullIndex);
	free(vehicleCollision->vehicleHullPlaneIndex);
	free(vehicleCollision->vehicleHullEmitStringIndex);
	free(vehicleCollision->vehicleHullSurfaceIndex);
	free(vehicleCollision->vehiclePolyListPlaneIndex);
	free(vehicleCollision->vehiclePolyListPointIndex);
	free(vehicleCollision->vehiclePolyListStringCharacter);
	free(vehicleCollision->vehicleNullSurface);
	free(vehicleCollision);
}