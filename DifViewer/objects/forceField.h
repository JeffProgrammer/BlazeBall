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

#ifndef forceField_h
#define forceField_h

#include "types.h"

typedef struct {
	U32 normalIndex;
	F32 planeDistance;
} Plane_FF;

typedef struct {
	U16 planeIndex;
	U16 frontIndex;
	U16 backIndex;
} BSPNode_FF;

typedef struct {
	U32 surfaceIndex;
	U16 surfaceCount;
} BSPSolidLeaf_FF;

typedef struct {
	U32 windingStart;
	U8 windingCount;
	U16 planeIndex;
	U8 surfaceFlags;
	U32 fanMask;
} Surface_FF;

typedef struct {
	U32 forceFieldFileVersion;
	String name;

	U32 numTriggers;
	String *trigger;

	BoxF boundingBox;
	SphereF boundingSphere;

	U32 numNormals;
	Point3F *normal;

	U32 numPlanes;
	Plane_FF *plane;

	U32 numBSPNodes;
	BSPNode_FF *BSPNode;

	U32 numBSPSolidLeaves;
	BSPSolidLeaf_FF *BSPSolidLeaf;

	U32 numWindings;
	U32 *index;

	U32 numSurfaces;
	Surface_FF *surface;

	U32 numSolidLeafSurfaces;
	U32 *solidLeafSurface;

	ColorI color;
} ForceField;

ForceField *forceField_read_file(FILE *file);
void forceField_release(ForceField *forceField);

#endif
