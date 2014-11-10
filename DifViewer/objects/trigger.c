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
#include "types.h"
#include "io.h"
#include "trigger.h"

Trigger *trigger_read_file(FILE *file) {
	Trigger *trigger = malloc(sizeof(Trigger));

	READTOVAR(trigger->name, String); //name
	READTOVAR(trigger->datablock, String); //datablock
	READTOVAR(trigger->properties, Dictionary); //properties
	READLOOPVAR(trigger->numPolyHedronPoints, trigger->polyHedronPoint, Point3F) {
		READTOVAR(trigger->polyHedronPoint[i], Point3F); //point
	}
	READLOOPVAR(trigger->numPolyHedronPlanes, trigger->polyHedronPlane, PlaneF) {
		READTOVAR(trigger->polyHedronPlane[i], PlaneF); //plane
	}
	READLOOPVAR(trigger->numPolyHedronEdges, trigger->polyHedronEdge, PolyHedronEdge) {
		READTOVAR(trigger->polyHedronEdge[i].face0, U32); //face0
		READTOVAR(trigger->polyHedronEdge[i].face1, U32); //face1
		READTOVAR(trigger->polyHedronEdge[i].vertex0, U32); //vertex0
		READTOVAR(trigger->polyHedronEdge[i].vertex1, U32); //vertex1
	}
	READTOVAR(trigger->offset, Point3F); //offset

	return trigger;
}

void trigger_release(Trigger *trigger) {
	releaseString(trigger->name);
	releaseString(trigger->datablock);
	releaseDictionary(trigger->properties);
	free(trigger->polyHedronPoint);
	free(trigger->polyHedronPlane);
	free(trigger->polyHedronEdge);
	free(trigger);
}
