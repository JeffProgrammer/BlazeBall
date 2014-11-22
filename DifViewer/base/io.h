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

#ifndef io_h
#define io_h

#include "types.h"

#define LIGHT_MAP_SIZE 0x400

/*
 Read number types from a file
 @var file - The FILE to read from (updates position)
 @return The number data at that position in the FILE
 */

//Unsigned ints
U64 readU64(FILE **file);
U32 readU32(FILE **file);
U16 readU16(FILE **file);
U8  readU8 (FILE **file);

//Signed ints
S64 readS64(FILE **file);
S32 readS32(FILE **file);
S16 readS16(FILE **file);
S8  readS8 (FILE **file);

//Floats
F32 readF32(FILE **file);

//Structures
PlaneF     readPlaneF(FILE **file);
Point3F    readPoint3F(FILE **file);
QuatF      readQuatF(FILE **file);
BoxF       readBoxF(FILE **file);
SphereF    readSphereF(FILE **file);
ColorI     readColorI(FILE **file);
String     readString(FILE **file);
PNG        readPNG(FILE **file);
Dictionary readDictionary(FILE **file);

/*
 Write number types to a file
 @var file - The FILE to read from (updates position)
 @return The number data at that position in the FILE
 */

//Unsigned ints
U32 writeU64(FILE **file, U64 value);
U32 writeU32(FILE **file, U32 value);
U32 writeU16(FILE **file, U16 value);
U32 writeU8 (FILE **file, U8  value);

//Signed ints
U32 writeS64(FILE **file, S64 value);
U32 writeS32(FILE **file, S32 value);
U32 writeS16(FILE **file, S16 value);
U32 writeS8 (FILE **file, S8  value);

//Floats
U32 writeF32(FILE **file, F32 value);

//Structures
U32 writePlaneF(FILE **file, PlaneF value);
U32 writePoint3F(FILE **file, Point3F value);
U32 writeQuatF(FILE **file, QuatF value);
U32 writeBoxF(FILE **file, BoxF value);
U32 writeSphereF(FILE **file, SphereF value);
U32 writeColorI(FILE **file, ColorI value);
U32 writeString(FILE **file, String value);
U32 writePNG(FILE **file, PNG value);
U32 writeDictionary(FILE **file, Dictionary value);

//Memory management
void releaseString(String string);
void releaseDictionary(Dictionary dictionary);

//Macros to speed up file reading
#define READ(type) read##type(&file)
#define READVAR(name, type) type name = read##type(&file)
#define READTOVAR(name, type) name = read##type(&file)
#define READCHECK(type, value) { if (READ(type) != value) return false; }

#define READLOOPVAR(countvar, listvar, type) \
countvar = readU32(&file); \
listvar = (type *)malloc(sizeof(type) * countvar); \
for (U32 i = 0; i < countvar; i ++)

#define READLOOP(name, type) \
type name##_length = read##type(&file); \
for (U32 i = 0; i < name##_length; i ++)

//Macros to speed up file reading
#define WRITE(type, value) write##type(&file, value)
#define WRITECHECK(type, value) { if (WRITE(type, value) != sizeof(type)) return 0; }

#define WRITELOOPVAR(type, countvar, listvar) \
WRITECHECK(U32, countvar);\
for (U32 i = 0; i < countvar; i ++) { \
	WRITECHECK(type, listvar[i]); \
}

#define WRITELOOPVARNOCHECK(type, countvar, listvar) \
WRITECHECK(U32, countvar);\
for (U32 i = 0; i < countvar; i ++) { \
	WRITE(type, listvar[i]); \
}

#define WRITELOOP(countvar) \
WRITECHECK(U32, countvar);\
for (U32 i = 0; i < countvar; i ++)\

bool isfile(String file);

#endif
