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
#include "dif.h"

DIF *dif_read_file(FILE *file) {
	//http://rustycode.com/tutorials/DIF_File_Format_44_14.html
	// Someone give that guy all the cookies.

	DIF *dif = malloc(sizeof(DIF));

	READCHECK(U32, 44); //interiorResourceFileVersion
	if (READ(U8)) { //previewIncluded
		READ(PNG); //previewBitmap
	}

	READLOOPVAR(dif->numDetailLevels, dif->interior, Interior *) {
		dif->interior[i] = interior_read_file(file);
	}
	READLOOPVAR(dif->numSubObjects, dif->subObject, Interior *) {
		dif->subObject[i] = interior_read_file(file);
	}
	READLOOPVAR(dif->numTriggers, dif->trigger, Trigger *) {
		dif->trigger[i] = trigger_read_file(file);
	}
	READLOOPVAR(dif->numInteriorPathFollowers, dif->interiorPathFollower, InteriorPathFollower *) {
		dif->interiorPathFollower[i] = interiorPathFollower_read_file(file);
	}
	READLOOPVAR(dif->numForceFields, dif->forceField, ForceField *) {
		dif->forceField[i] = forceField_read_file(file);
	}
	READLOOPVAR(dif->numAISpecialNodes, dif->aiSpecialNode, AISpecialNode *) {
		dif->aiSpecialNode[i] = aiSpecialNode_read_file(file);
	}
	if (READ(U32) == 1) { //readVehicleCollision
		dif->vehicleCollision = vehicleCollision_read_file(file);
	}
	READ(U32); //unknown
	READ(U32); //unknown
	READ(U32); //unknown
	READ(U32); //unknown
	if (READ(U32) == 2) { //readGameEntities
		READLOOPVAR(dif->numGameEntities, dif->gameEntity, GameEntity *) {
			dif->gameEntity[i] = gameEntity_read_file(file);
		}
	}
	READ(U32); //dummy

	return dif;
}

void dif_release(DIF *dif) {
	for (U32 i = 0; i < dif->numDetailLevels; i ++) {
		interior_release(dif->interior[i]);
	}
	free(dif->interior);

	for (U32 i = 0; i < dif->numSubObjects; i ++) {
		interior_release(dif->subObject[i]);
	}
	free(dif->subObject);

	for (U32 i = 0; i < dif->numTriggers; i ++) {
		trigger_release(dif->trigger[i]);
	}
	free(dif->trigger);

	for (U32 i = 0; i < dif->numInteriorPathFollowers; i ++) {
		interiorPathFollower_release(dif->interiorPathFollower[i]);
	}
	free(dif->interiorPathFollower);

	for (U32 i = 0; i < dif->numForceFields; i ++) {
		forceField_release(dif->forceField[i]);
	}
	free(dif->forceField);

	for (U32 i = 0; i < dif->numAISpecialNodes; i ++) {
		aiSpecialNode_release(dif->aiSpecialNode[i]);
	}
	free(dif->aiSpecialNode);

	vehicleCollision_release(dif->vehicleCollision);

	for (U32 i = 0; i < dif->numGameEntities; i ++) {
		gameEntity_release(dif->gameEntity[i]);
	}
	free(dif->gameEntity);

	free(dif);
}
