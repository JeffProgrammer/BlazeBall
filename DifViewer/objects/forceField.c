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
//  * Neither the name of the <organization> nor the
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
	READTOVAR(forceField->color, ColorI); //color

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
