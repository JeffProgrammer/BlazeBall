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
#include <string.h>
#include <math.h>
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

PNG readPNG(FILE **file) {
	U8 PNGFooter[8] = {0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82};
	PNG value;
	value.data = calloc(sizeof(U8), LIGHT_MAP_SIZE);

	//I can't parse these, so I just read em all
	value.size = 0;
	for (int i = 0; i < sizeof(PNGFooter) / sizeof(PNGFooter[0]); i ++) {
		while ((value.data[value.size ++] = readU8(file)) != PNGFooter[i]);
	}
	return value;
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

//-----------------------------------------------------------------------------

U32 writeU64(FILE **file, U64 value) {
	fpos_t pos;
	fgetpos(*file, &pos);
	DEBUG_PRINT("Write U64 %08llX: %llu\n", pos, value);

	return (U32)fwrite(&value, sizeof(value), 1, *file) * sizeof(value);
}
U32 writeU32(FILE **file, U32 value) {
	fpos_t pos;
	fgetpos(*file, &pos);
	DEBUG_PRINT("Write U32 %08llX: %u\n", pos, value);

	return (U32)fwrite(&value, sizeof(value), 1, *file) * sizeof(value);
}
U32 writeU16(FILE **file, U16 value) {
	fpos_t pos;
	fgetpos(*file, &pos);
	DEBUG_PRINT("Write U16 %08llX: %hu\n", pos, value);

	return (U32)fwrite(&value, sizeof(value), 1, *file) * sizeof(value);
}
U32 writeU8(FILE **file, U8 value) {
	fpos_t pos;
	fgetpos(*file, &pos);
	DEBUG_PRINT("Write U8 %08llX: %u\n", pos, value);

	return (U32)fwrite(&value, sizeof(value), 1, *file) * sizeof(value);
}
U32 writeF32(FILE **file, F32 value) {
	fpos_t pos;
	fgetpos(*file, &pos);
	DEBUG_PRINT("Write F32 %08llX: %f\n", pos, value);

	return (U32)fwrite(&value, sizeof(value), 1, *file) * sizeof(value);
}

//Lazy!
U32 writeS64(FILE **file, S64 value) { return writeU64(file, value); }
U32 writeS32(FILE **file, S32 value) { return writeU32(file, value); }
U32 writeS16(FILE **file, S16 value) { return writeU16(file, value); }
U32 writeS8 (FILE **file, S8  value) { return writeU8 (file, value); }

U32 writePlaneF(FILE **file, PlaneF value) {
	U32 count = 0;
	count += writeF32(file, value.x);
	count += writeF32(file, value.y);
	count += writeF32(file, value.z);
	count += writeF32(file, value.d);
	return count;
}

U32 writePoint3F(FILE **file, Point3F value) {
	U32 count = 0;
	count += writeF32(file, value.x);
	count += writeF32(file, value.y);
	count += writeF32(file, value.z);
	return count;
}

U32 writeQuatF(FILE **file, QuatF value) {
	U32 count = 0;
	count += writeF32(file, value.w);
	count += writeF32(file, value.x);
	count += writeF32(file, value.y);
	count += writeF32(file, value.z);
	return count;
}

U32 writeBoxF(FILE **file, BoxF value) {
	U32 count = 0;
	count += writeF32(file, value.minX);
	count += writeF32(file, value.minY);
	count += writeF32(file, value.minZ);
	count += writeF32(file, value.maxX);
	count += writeF32(file, value.maxY);
	count += writeF32(file, value.maxZ);
	return count;
}

U32 writeSphereF(FILE **file, SphereF value) {
	U32 count = 0;
	count += writeF32(file, value.x);
	count += writeF32(file, value.y);
	count += writeF32(file, value.z);
	count += writeF32(file, value.radius);
	return count;
}

U32 writeColorI(FILE **file, ColorI value) {
	U32 count = 0;
	count += writeU8(file, value.red);
	count += writeU8(file, value.green);
	count += writeU8(file, value.blue);
	count += writeU8(file, value.alpha);
	return count;
}

U32 writeString(FILE **file, String value) {
	//<length><bytes>
	U32 count = 0;

	count += writeU8(file, strlen((const char *)value));
	for (int i = 0; i < strlen((const char *)value); i ++) {
		count += writeU8(file, value[i]);
	}
	return count;
}

U32 writePNG(FILE **file, PNG value) {
	//Basically dump out everything. Yeah.

	U32 count = 0;
	for (U32 i = 0; i < value.size; i ++) {
		count += writeU8(file, value.data[i]);
	}
	return count;
}

U32 writeDictionary(FILE **file, Dictionary value) {
	//<length>[<name><value>]...
	U32 count = 0;

	count += writeU32(file, value.size);
	for (int i = 0; i < value.size; i ++) {
		count += writeString(file, value.names[i]);
		count += writeString(file, value.values[i]);
	}

	return count;
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
