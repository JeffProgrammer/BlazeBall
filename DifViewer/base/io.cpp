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

#if 1
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

IO *IO::getIO() {
	static IO *gIO;

	if (gIO == nullptr) {
		gIO = new IO;
	}
	return gIO;
}

IO::IO() {

}

IO::~IO() {
	
}

void IO::reverse(FILE **file, U32 bytes) {
	fpos_t pos = ftell(*file);
	pos -= bytes;
	fsetpos(*file, &pos);
}

bool IO::read(FILE *file, U64 *value, String name) {
	fpos_t pos;
	fgetpos(file, &pos);
	fread(value, sizeof(*value), 1, file);

	DEBUG_PRINT("Read U64 (%s) 0x%08llX %lld: 0x%016llX / %llu\n", (const char *)name, pos, pos, __builtin_bswap64(*value), *value);
	return true;
}
bool IO::read(FILE *file, U32 *value, String name) {
	fpos_t pos;
	fgetpos(file, &pos);
	fread(value, sizeof(*value), 1, file);

	DEBUG_PRINT("Read U32 (%s) 0x%08llX %lld: 0x%08X / %u\n", (const char *)name, pos, pos, __builtin_bswap32(*value), *value);
	return true;
}
bool IO::read(FILE *file, U16 *value, String name) {
	fpos_t pos;
	fgetpos(file, &pos);
	fread(value, sizeof(*value), 1, file);

	DEBUG_PRINT("Read U16 (%s) 0x%08llX %lld: 0x%04hX / %hu\n", (const char *)name, pos, pos, _OSSwapInt16(*value), *value);
	return true;
}
bool IO::read(FILE *file, U8 *value, String name) {
	fpos_t pos;
	fgetpos(file, &pos);
	fread(value, sizeof(*value), 1, file);

	DEBUG_PRINT("Read U8 (%s) 0x%08llX %lld: 0x%02hhX / %u\n", (const char *)name, pos, pos, *value, *value);
	return true;
}
bool IO::read(FILE *file, F32 *value, String name) {
	fpos_t pos;
	fgetpos(file, &pos);
	fread(value, sizeof(*value), 1, file);

	DEBUG_PRINT("Read F32 (%s) 0x%08llX %lld: 0x%08X %f\n", (const char *)name, pos, pos, __builtin_bswap32(*(U32 *)value), *value);
	return true;
}

//Lazy!
bool IO::read(FILE *file, S64 *value, String name) { return read(file, (U64 *)value, name); }
bool IO::read(FILE *file, S32 *value, String name) { return read(file, (U32 *)value, name); }
bool IO::read(FILE *file, S16 *value, String name) { return read(file, (U16 *)value, name); }
bool IO::read(FILE *file, S8  *value, String name) { return read(file, (U8  *)value, name); }

bool PlaneF::read(FILE *file) {
	return
		io->read(file, &x, (String)"x") &&
		io->read(file, &y, (String)"x") &&
		io->read(file, &z, (String)"x") &&
		io->read(file, &d, (String)"x");
}

bool QuatF::read(FILE *file) {
	return
		io->read(file, &w, (String)"w") &&
		io->read(file, &x, (String)"x") &&
		io->read(file, &y, (String)"y") &&
		io->read(file, &z, (String)"z");
}

bool BoxF::read(FILE *file) {
	return
		io->read(file, &minX, (String)"minX") &&
		io->read(file, &minY, (String)"minY") &&
		io->read(file, &minZ, (String)"minZ") &&
		io->read(file, &maxX, (String)"maxX") &&
		io->read(file, &maxY, (String)"maxY") &&
		io->read(file, &maxZ, (String)"maxZ");
}

bool SphereF::read(FILE *file) {
	return
		io->read(file, &x, (String)"x") &&
		io->read(file, &y, (String)"y") &&
		io->read(file, &z, (String)"z") &&
		io->read(file, &radius, (String)"radius");
}

bool String::read(FILE *file) {
	//<length><bytes>

	io->read(file, (U8 *)&length, "length");
	data = new U8[length + 1];
	for (int i = 0; i < length; i ++) {
		io->read(file, &(data[i]), "data");
	}
	//Null-terminate
	data[length] = 0;
	return true;
}

bool PNG::read(FILE *file) {
	U8 PNGFooter[8] = {0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82};
	data = new U8[LIGHT_MAP_SIZE];

	//I can't parse these, so I just read em all
	size = 0;
	for (U32 i = 0; ;i ++) {
		io->read(file, &(data[i]), "data");
		if (i > 8 && memcmp(&data[i - 7], PNGFooter, 8) == 0)
			break;
	}

	return true;
}

bool Dictionary::read(FILE *file) {
	//<length>[<name><value>]...
	io->read(file, (U8 *)&size, "size");
	names = new String*[size];
	values = new String*[size];

	for (int i = 0; i < size; i ++) {
		names[i] = new String();
		values[i] = new String();
		names[i]->read(file);
		values[i]->read(file);
	}

	return true;
}

//-----------------------------------------------------------------------------

bool IO::write(FILE *file, U64 value) {
	fpos_t pos;
	fgetpos(file, &pos);
	DEBUG_PRINT("Write U64 %08llX: %llu\n", pos, value);
	fwrite(&value, sizeof(value), 1, file);
	return true;
}
bool IO::write(FILE *file, U32 value) {
	fpos_t pos;
	fgetpos(file, &pos);
	DEBUG_PRINT("Write U32 %08llX: %u\n", pos, value);
	fwrite(&value, sizeof(value), 1, file);
	return true;
}
bool IO::write(FILE *file, U16 value) {
	fpos_t pos;
	fgetpos(file, &pos);
	DEBUG_PRINT("Write U16 %08llX: %hu\n", pos, value);
	fwrite(&value, sizeof(value), 1, file);
	return true;
}
bool IO::write(FILE *file, U8 value) {
	fpos_t pos;
	fgetpos(file, &pos);
	DEBUG_PRINT("Write U8 %08llX: %u\n", pos, value);
	fwrite(&value, sizeof(value), 1, file);
	return true;
}
bool IO::write(FILE *file, F32 value) {
	fpos_t pos;
	fgetpos(file, &pos);
	DEBUG_PRINT("Write F32 %08llX: %f\n", pos, value);
	fwrite(&value, sizeof(value), 1, file);
	return true;
}

//Lazy!
bool IO::write(FILE *file, S64 value) { return write(file, (U64)value); }
bool IO::write(FILE *file, S32 value) { return write(file, (U32)value); }
bool IO::write(FILE *file, S16 value) { return write(file, (U16)value); }
bool IO::write(FILE *file, S8  value) { return write(file, (U8) value); }

bool PlaneF::write(FILE *file) {
	return
		io->write(file, x) &&
		io->write(file, y) &&
		io->write(file, z) &&
		io->write(file, d);
}

template <typename T>
bool Point2<T>::write(FILE *file) {
	return
		io->write(file, x) &&
		io->write(file, y);
}

template <typename T>
bool Point3<T>::write(FILE *file) {
	return
		io->write(file, x) &&
		io->write(file, y) &&
		io->write(file, z);
}

template <typename T>
bool Point4<T>::write(FILE *file) {
	return
		io->write(file, w) &&
		io->write(file, x) &&
		io->write(file, y) &&
		io->write(file, z);
}

bool QuatF::write(FILE *file) {
	return
		io->write(file, w) &&
		io->write(file, x) &&
		io->write(file, y) &&
		io->write(file, z);
}

bool BoxF::write(FILE *file) {
	return
		io->write(file, minX) &&
		io->write(file, minY) &&
		io->write(file, minZ) &&
		io->write(file, maxX) &&
		io->write(file, maxY) &&
		io->write(file, maxZ);
}

bool SphereF::write(FILE *file) {
	return
		io->write(file, x) &&
		io->write(file, y) &&
		io->write(file, z) &&
		io->write(file, radius);
}

template <typename T>
bool Color<T>::write(FILE *file) {
	return
		io->write(file, red);
		io->write(file, green);
		io->write(file, blue);
		io->write(file, alpha);
}

bool String::write(FILE *file) {
	//<length><bytes>
	if (!io->write(file, (U8)length))
		return false;
	for (int i = 0; i < length; i ++) {
		if (!io->write(file, data[i]))
			return false;
	}
	return true;
}

bool PNG::write(FILE *file) {
	//Basically dump out everything. Yeah.

	for (U32 i = 0; i < size; i ++) {
		if (!io->write(file, data[i]))
			return false;
	}
	return true;
}

bool Dictionary::write(FILE *file) {
	//<length>[<name><value>]...

	if (!io->write(file, size))
		return false;
	for (int i = 0; i < size; i ++) {
		if (!io->write(file, *names[i]) &&
		    !io->write(file, *values[i]))
		return false;
	}

	return true;
}

bool IO::write(FILE *file, PlaneF value) {
	return value.write(file);
}
bool IO::write(FILE *file, Point3F value) {
	return value.write(file);
}
bool IO::write(FILE *file, QuatF value) {
	return value.write(file);
}
bool IO::write(FILE *file, BoxF value) {
	return value.write(file);
}
bool IO::write(FILE *file, SphereF value) {
	return value.write(file);
}
bool IO::write(FILE *file, ColorI value) {
	return value.write(file);
}
bool IO::write(FILE *file, String value) {
	return value.write(file);
}
bool IO::write(FILE *file, PNG value) {
	return value.write(file);
}
bool IO::write(FILE *file, Dictionary value) {
	return value.write(file);
}

//Mem mgt
void releaseString(String string) {
	free(string);
}

void releaseDictionary(Dictionary dictionary) {
	for (int i = 0; i < dictionary.size; i ++) {
		releaseString(*dictionary.names[i]);
		releaseString(*dictionary.values[i]);
	}
	free(dictionary.names);
	free(dictionary.values);
}

bool IO::isfile(String *file) {
	FILE *stream = fopen((const char *)file->data, "r");
	if (stream) {
		fclose(stream);
		return true;
	}
	return false;
}
