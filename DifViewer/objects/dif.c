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
#include "dif.h"

DIF *dif_read_file(FILE *file, String directory) {
	//http://rustycode.com/tutorials/DIF_File_Format_44_14.html
	// Someone give that guy all the cookies.

	DIF *dif = malloc(sizeof(DIF));

	READCHECK(U32, 44); //interiorResourceFileVersion
	if (READ(U8)) { //previewIncluded
		READ(PNG); //previewBitmap
	}

	READLOOPVAR(dif->numDetailLevels, dif->interior, Interior *) {
		dif->interior[i] = interior_read_file(file, directory);
	}
	READLOOPVAR(dif->numSubObjects, dif->subObject, Interior *) {
		dif->subObject[i] = interior_read_file(file, directory);
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
	} else {
		dif->numGameEntities = 0;
		dif->gameEntity = NULL;
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
