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
#include "gameEntity.h"

GameEntity *gameEntity_read_file(FILE *file) {
	GameEntity *gameEntity = malloc(sizeof(GameEntity));

	READTOVAR(gameEntity->datablock, String); //datablock
	READTOVAR(gameEntity->gameClass, String); //gameClass
	READTOVAR(gameEntity->position, Point3F); //position
	READTOVAR(gameEntity->properties, Dictionary); //properties

	return gameEntity;
}

void gameEntity_release(GameEntity *gameEntity) {
	releaseString(gameEntity->datablock);
	releaseString(gameEntity->gameClass);
	releaseDictionary(gameEntity->properties);
	free(gameEntity);
}
