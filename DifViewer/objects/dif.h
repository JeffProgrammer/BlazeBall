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

#ifndef dif_h
#define dif_h

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"
#include "interior.h"
#include "trigger.h"
#include "interiorPathFollower.h"
#include "forceField.h"
#include "aiSpecialNode.h"
#include "vehicleCollision.h"
#include "gameEntity.h"

typedef struct {
	U32 numDetailLevels;
	Interior **interior;

	U32 numSubObjects;
	Interior **subObject;

	U32 numTriggers;
	Trigger **trigger;

	U32 numInteriorPathFollowers;
	InteriorPathFollower **interiorPathFollower;

	U32 numForceFields;
	ForceField **forceField;

	U32 numAISpecialNodes;
	AISpecialNode **aiSpecialNode;

	U32 readVehicleCollision;
	VehicleCollision *vehicleCollision;

	U32 unknown0;
	U32 unknown1;
	U32 unknown2;
	U32 unknown3;

	U32 readGameEntities;
	U32 numGameEntities;
	GameEntity **gameEntity;

	U32 dummy;
} DIF;

/**
 Reads a DIF from a FILE
 @arg file - The FILE to read from (updates position)
 @return A DIF
 */
DIF *dif_read_file(FILE *file);

/**
 Frees the DIF and all memory contained within it
 @arg dif - The DIF to release
 */
void dif_release(DIF *dif);

#ifdef __cplusplus
}
#endif

#endif
