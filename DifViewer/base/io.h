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

#ifndef io_h
#define io_h

#include "types.h"

//Base types
U64 readU64(FILE **file);
U32 readU32(FILE **file);
U16 readU16(FILE **file);
U8  readU8 (FILE **file);
F32 readF32(FILE **file);

S64 readS64(FILE **file);
S32 readS32(FILE **file);
S16 readS16(FILE **file);
S8  readS8 (FILE **file);

//Structures
PlaneF     readPlaneF(FILE **file);
Point3F    readPoint3F(FILE **file);
QuatF      readQuatF(FILE **file);
BoxF       readBoxF(FILE **file);
SphereF    readSphereF(FILE **file);
ColorF     readColorF(FILE **file);
String     readString(FILE **file);
void       readPNG(FILE **file);
Dictionary readDictionary(FILE **file);

//Mem mgt
void releaseString(String string);
void releaseDictionary(Dictionary dictionary);

//Warning: Lazy!

#define READ(type) read##type(&file)
#define READVAR(name, type) type name = read##type(&file)
#define READTOVAR(name, type) name = read##type(&file)
#define READCHECK(type, value) { if (READ(type) != value) return 0; }

#define READLOOPVAR(countvar, listvar, type) \
countvar = readU32(&file); \
listvar = (type *)malloc(sizeof(type) * countvar); \
	for (U32 i = 0; i < countvar; i ++)

#define READLOOP(name, type) \
type name##_length = read##type(&file); \
	for (U32 i = 0; i < name##_length; i ++)

#endif
