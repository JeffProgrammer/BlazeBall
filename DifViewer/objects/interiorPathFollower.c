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
