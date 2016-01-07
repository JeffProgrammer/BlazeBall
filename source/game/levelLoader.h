//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GAME_LEVELLOADER_H_
#define _GAME_LEVELLOADER_H_

#include <string>
#include "game/world.h"

bool loadLevel(World *world, const std::string &file);

#endif // _GAME_LEVELLOADER_H_