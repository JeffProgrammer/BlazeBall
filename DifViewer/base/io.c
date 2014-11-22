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

#include <stdio.h>
#include <stdlib.h>
#include "io.h"

#if 0
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

U64 readU64(FILE **file) {
	U64 value = -1;
	fpos_t pos;
	fgetpos(*file, &pos);
	fread(&value, sizeof(value), 1, *file);

	DEBUG_PRINT("Read U64 %08llX: %llu\n", pos, value);

	return value;
}
U32 readU32(FILE **file) {
	U32 value = -1;
	fpos_t pos;
	fgetpos(*file, &pos);
	fread(&value, sizeof(value), 1, *file);

	DEBUG_PRINT("Read U32 %08llX: %u\n", pos, value);

	return value;
}
U16 readU16(FILE **file) {
	U16 value = -1;
	fpos_t pos;
	fgetpos(*file, &pos);
	fread(&value, sizeof(value), 1, *file);

	DEBUG_PRINT("Read U16 %08llX: %hu\n", pos, value);

	return value;
}
U8 readU8(FILE **file) {
	U8 value = -1;
	fpos_t pos;
	fgetpos(*file, &pos);
	fread(&value, sizeof(value), 1, *file);

	DEBUG_PRINT("Read U8 %08llX: %u\n", pos, value);

	return value;
}
F32 readF32(FILE **file) {
	F32 value = -1;
	fpos_t pos;
	fgetpos(*file, &pos);
	fread(&value, sizeof(value), 1, *file);

	DEBUG_PRINT("Read F32 %08llX: %f\n", pos, value);

	return value;
}

//Lazy!
S64 readS64(FILE **file) { return (S64)readU64(file); }
S32 readS32(FILE **file) { return (S32)readU32(file); }
S16 readS16(FILE **file) { return (S16)readU16(file); }
S8  readS8 (FILE **file) { return  (S8)readU8 (file); }

PlaneF readPlaneF(FILE **file) {
	PlaneF value;
	value.x = readF32(file);
	value.y = readF32(file);
	value.z = readF32(file);
	value.d = readF32(file);
	return value;
}

Point3F readPoint3F(FILE **file) {
	Point3F value;
	value.x = readF32(file);
	value.y = readF32(file);
	value.z = readF32(file);
	return value;
}

QuatF readQuatF(FILE **file) {
	QuatF value;
	value.w = readF32(file);
	value.x = readF32(file);
	value.y = readF32(file);
	value.z = readF32(file);
	return value;
}

BoxF readBoxF(FILE **file) {
	BoxF value;
	value.minX = readF32(file);
	value.minY = readF32(file);
	value.minZ = readF32(file);
	value.maxX = readF32(file);
	value.maxY = readF32(file);
	value.maxZ = readF32(file);
	return value;
}

SphereF readSphereF(FILE **file) {
	SphereF value;
	value.x = readF32(file);
	value.y = readF32(file);
	value.z = readF32(file);
	value.radius = readF32(file);
	return value;
}

ColorI readColorI(FILE **file) {
	ColorI value;
	value.red = readU8(file);
	value.green = readU8(file);
	value.blue = readU8(file);
	value.alpha = readU8(file);
	return value;
}

String readString(FILE **file) {
	//<length><bytes>

	U8 length = readU8(file);
	String value = (String)malloc(length + 1);
	for (int i = 0; i < length; i ++) {
		value[i] = readU8(file);
	}
	//Null-terminate
	value[length] = 0;
	return value;
}

void readPNG(FILE **file) {
	U8 PNGFooter[8] = {0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82};

	//Basically throw out everything. Yeah.
	for (int i = 0; i < sizeof(PNGFooter) / sizeof(PNGFooter[0]); i ++) {
		while (readU8(file) != PNGFooter[i]);
	}
}

Dictionary readDictionary(FILE **file) {
	//<length>[<name><value>]...
	Dictionary value;
	U32 size = readU32(file);
	value.size = size;
	value.names = malloc(sizeof(String) * size);
	value.values = malloc(sizeof(String) * size);

	for (int i = 0; i < size; i ++) {
		value.names[i]  = readString(file);
		value.values[i] = readString(file);
	}

	return value;
}

//Mem mgt
void releaseString(String string) {
	free(string);
}

void releaseDictionary(Dictionary dictionary) {
	for (int i = 0; i < dictionary.size; i ++) {
		releaseString(dictionary.names[i]);
		releaseString(dictionary.values[i]);
	}
	free(dictionary.names);
	free(dictionary.values);
}

bool isfile(String file) {
	FILE *stream = fopen((const char *)file, "r");
	if (stream) {
		fclose(stream);
		return true;
	}
	return false;
}
