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
#include "forceField.h"

ForceField *forceField_read_file(FILE *file) {
	ForceField *forceField = malloc(sizeof(ForceField));

	READTOVAR(forceField->forceFieldFileVersion, U32); //forceFieldFileVersion
	READTOVAR(forceField->name, String); //name
	READLOOPVAR(forceField->numTriggers, forceField->trigger, String) {
		READTOVAR(forceField->trigger[i], String); //trigger
	}
	READTOVAR(forceField->boundingBox, BoxF); //boundingBox
	READTOVAR(forceField->boundingSphere, SphereF); //boundingSphere
	READLOOPVAR(forceField->numNormals, forceField->normal, Point3F) {
		READTOVAR(forceField->normal[i], Point3F); //normal
	}
	READLOOPVAR(forceField->numPlanes, forceField->plane, Plane_FF) {
		READTOVAR(forceField->plane[i].normalIndex, U32); //normalIndex
		READTOVAR(forceField->plane[i].planeDistance, F32); //planeDistance
	}
	READLOOPVAR(forceField->numBSPNodes, forceField->BSPNode, BSPNode_FF) {
		READTOVAR(forceField->BSPNode[i].planeIndex, U16); //planeIndex
		READTOVAR(forceField->BSPNode[i].frontIndex, U16); //frontIndex
		READTOVAR(forceField->BSPNode[i].backIndex, U16); //backIndex
	}
	READLOOPVAR(forceField->numBSPSolidLeaves, forceField->BSPSolidLeaf, BSPSolidLeaf_FF) {
		READTOVAR(forceField->BSPSolidLeaf[i].surfaceIndex, U32); //surfaceIndex
		READTOVAR(forceField->BSPSolidLeaf[i].surfaceCount, U16); //surfaceCount
	}
	READLOOPVAR(forceField->numWindings, forceField->index, U32) {
		READTOVAR(forceField->index[i], U32); //index
	}
	READLOOPVAR(forceField->numSurfaces, forceField->surface, Surface_FF) {
		READTOVAR(forceField->surface[i].windingStart, U32); //windingStart
		READTOVAR(forceField->surface[i].windingCount, U8); //windingCount
		READTOVAR(forceField->surface[i].planeIndex, U16); //planeIndex
		READTOVAR(forceField->surface[i].surfaceFlags, U8); //surfaceFlags
		READTOVAR(forceField->surface[i].fanMask, U32); //fanMask
	}
	READLOOPVAR(forceField->numSolidLeafSurfaces, forceField->solidLeafSurface, U32) {
		READTOVAR(forceField->solidLeafSurface[i], U32); //solidLeafSurface
	}
	READTOVAR(forceField->color, ColorF); //color

	return forceField;
}

void forceField_release(ForceField *forceField) {
	releaseString(forceField->name);
	for (U32 i = 0; i < forceField->numTriggers; i ++) {
		releaseString(forceField->trigger[i]);
	}
	free(forceField->trigger);
	free(forceField->normal);
	free(forceField->plane);
	free(forceField->BSPNode);
	free(forceField->BSPSolidLeaf);
	free(forceField->index);
	free(forceField->surface);
	free(forceField->solidLeafSurface);
	free(forceField);
}
