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
#include "interiorPathFollower.h"

InteriorPathFollower *interiorPathFollower_read_file(FILE *file) {
	InteriorPathFollower *interiorPathFollower = malloc(sizeof(InteriorPathFollower));

	READTOVAR(interiorPathFollower->name, String); //name
	READTOVAR(interiorPathFollower->datablock, String); //datablock
	READTOVAR(interiorPathFollower->interiorResIndex, U32); //interiorResIndex
	READTOVAR(interiorPathFollower->offset, Point3F); //offset
	READTOVAR(interiorPathFollower->properties, Dictionary); //properties
	READLOOPVAR(interiorPathFollower->numTriggerIds, interiorPathFollower->triggerId, U32) {
		READTOVAR(interiorPathFollower->triggerId[i], U32); //triggerId
	}
	READLOOPVAR(interiorPathFollower->numWayPoints, interiorPathFollower->wayPoint, WayPoint) {
		READTOVAR(interiorPathFollower->wayPoint[i].position, Point3F); //position
		READTOVAR(interiorPathFollower->wayPoint[i].rotation, QuatF); //rotation
		READTOVAR(interiorPathFollower->wayPoint[i].msToNext, U32); //msToNext
		READTOVAR(interiorPathFollower->wayPoint[i].smoothingType, U32); //smoothingType
	}
	READTOVAR(interiorPathFollower->totalMS, U32); //totalMS

	return interiorPathFollower;
}

void interiorPathFollower_release(InteriorPathFollower *interiorPathFollower) {
	releaseString(interiorPathFollower->name);
	releaseString(interiorPathFollower->datablock);
	releaseDictionary(interiorPathFollower->properties);

	free(interiorPathFollower->triggerId);
	free(interiorPathFollower->wayPoint);

	free(interiorPathFollower);
}
