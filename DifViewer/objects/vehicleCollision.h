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

#ifndef vehicleCollision_h
#define vehicleCollision_h

#include "types.h"

typedef struct {
	U32 hullStart;
	U16 hullCount;
	F32 minX;
	F32 maxX;
	F32 minY;
	F32 maxY;
	F32 minZ;
	F32 maxZ;
	U32 surfaceStart;
	U16 surfaceCount;
	U32 planeStart;
	U32 polyListPlaneStart;
	U32 polyListPointStart;
	U32 polyListStringStart;
} VehicleConvexHull;

typedef struct {
	U32 windingStart;
	U16 planeIndex;
	U8 surfaceFlags;
	U32 windingCount;
} VehicleNullSurface;

typedef struct {
	U32 vehicleCollisionFileVersion;

	U32 numVehicleConvexHulls;
	VehicleConvexHull *vehicleConvexHull;

	U32 numVehicleConvexHullEmitStrings;
	U8 *vehicleConvexHullEmitStringCharacter;

	U32 numVehicleHullIndices;
	U32 *vehicleHullIndex;

	U32 numVehicleHullPlaneIndices;
	U16 *vehicleHullPlaneIndex;

	U32 numVehicleHullEmitStringIndices;
	U32 *vehicleHullEmitStringIndex;

	U32 numVehicleHullSurfaceIndices;
	U32 *vehicleHullSurfaceIndex;

	U32 numVehiclePolyListPlanes;
	U16 *vehiclePolyListPlaneIndex;

	U32 numVehiclePolyListPoints;
	U32 *vehiclePolyListPointIndex;

	U32 numVehiclePolyListStrings;
	U8 *vehiclePolyListStringCharacter;

	U32 numVehicleNullSurfaces;
	VehicleNullSurface *vehicleNullSurface;
} VehicleCollision;

VehicleCollision *vehicleCollision_read_file(FILE *file);
void vehicleCollision_release(VehicleCollision *vehicleCollision);

#endif
