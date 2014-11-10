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