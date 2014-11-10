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

S64 readS64(FILE **file) { return (S64)readU64(file); }
S32 readS32(FILE **file) { return (S32)readU32(file); }
S16 readS16(FILE **file) { return (S16)readU16(file); }
S8  readS8 (FILE **file) { return  (S8)readU8 (file); }

PlaneF readPlaneF(FILE **file) {
	PlaneF value;
	value.index1 = readU32(file);
	value.distance1 = readF32(file);
	value.index2 = readU32(file);
	value.distance2 = readF32(file);
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

ColorF readColorF(FILE **file) {
	ColorF value;
	value.red = readU8(file);
	value.green = readU8(file);
	value.blue = readU8(file);
	value.alpha = readU8(file);
	return value;
}

String readString(FILE **file) {
	U8 length = readU8(file);
	String value = (String)malloc(length + 1);
	for (int i = 0; i < length; i ++) {
		value[i] = readU8(file);
	}
	value[length] = 0;
	return value;
}

void readPNG(FILE **file) {
	U8 PNGFooter[8] = {0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82};

	for (int i = 0; i < sizeof(PNGFooter) / sizeof(PNGFooter[0]); i ++) {
		while (readU8(file) != PNGFooter[i]);
	}
}

Dictionary readDictionary(FILE **file) {
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
