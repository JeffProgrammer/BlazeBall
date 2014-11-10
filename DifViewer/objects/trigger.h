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

#ifndef trigger_h
#define trigger_h

#include "types.h"

typedef struct {
	U32 face0;
	U32 face1;
	U32 vertex0;
	U32 vertex1;
} PolyHedronEdge;

typedef struct {
	String name;
	String datablock;
	Dictionary properties;

	U32 numPolyHedronPoints;
	Point3F *polyHedronPoint;

	U32 numPolyHedronPlanes;
	PlaneF *polyHedronPlane;

	U32 numPolyHedronEdges;
	PolyHedronEdge *polyHedronEdge;

	Point3F offset;
} Trigger;

Trigger *trigger_read_file(FILE *file);
void trigger_release(Trigger *trigger);

#endif
