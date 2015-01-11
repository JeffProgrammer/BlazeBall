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
#define io IO::getIO()

/*
 Read number types from a file
 @var file - The FILE to read from (updates position)
 @return The number data at that position in the FILE
 */
class IO {
public:
	IO();
	~IO();

	static IO *getIO();

	void reverse(FILE **file, U32 bytes);

	//Unsigned ints
	bool read(FILE *file, U64 *value, String name);
	bool read(FILE *file, U32 *value, String name);
	bool read(FILE *file, U16 *value, String name);
	bool read(FILE *file,  U8 *value, String name);
	
	//Signed ints
	bool read(FILE *file, S64 *value, String name);
	bool read(FILE *file, S32 *value, String name);
	bool read(FILE *file, S16 *value, String name);
	bool read(FILE *file,  S8 *value, String name);

	//Floats
	bool read(FILE *file, F32 *value, String name);

	//Structures
	bool read(FILE *file, PlaneF     *value, String name);
	bool read(FILE *file, Point3F    *value, String name);
	bool read(FILE *file, QuatF      *value, String name);
	bool read(FILE *file, BoxF       *value, String name);
	bool read(FILE *file, SphereF    *value, String name);
	bool read(FILE *file, ColorI     *value, String name);
	bool read(FILE *file, String     *value, String name);
	bool read(FILE *file, PNG        *value, String name);
	bool read(FILE *file, Dictionary *value, String name);

	/*
	 Write number types to a file
	 @var file - The FILE to read from (updates position)
	 @return The number data at that position in the FILE
	 */

	//Unsigned ints
	bool write(FILE *file, U64 value);
	bool write(FILE *file, U32 value);
	bool write(FILE *file, U16 value);
	bool write(FILE *file, U8  value);

	//Signed ints
	bool write(FILE *file, S64 value);
	bool write(FILE *file, S32 value);
	bool write(FILE *file, S16 value);
	bool write(FILE *file, S8  value);

	//Floats
	bool write(FILE *file, F32 value);

	//Structures
	bool write(FILE *file, PlaneF value);
	bool write(FILE *file, Point3F value);
	bool write(FILE *file, QuatF value);
	bool write(FILE *file, BoxF value);
	bool write(FILE *file, SphereF value);
	bool write(FILE *file, ColorI value);
	bool write(FILE *file, String value);
	bool write(FILE *file, PNG value);
	bool write(FILE *file, Dictionary value);

	bool isfile(String *file);
};

//Memory management
void releaseString(String string);
void releaseDictionary(Dictionary dictionary);

//Macros to speed up file reading
#define REVERSE(size) io->reverse(&file, size)

//Hack to get the read() macro to return a value from a function that uses a ref
template <typename T>
inline T read(FILE *file, T *thing) {
	T __garbage;
	io->read(file, &__garbage, (String)"garbage");
	return __garbage;
}
//I'm so sorry about (type *)file, but that's the only way to get C++ to interpret
// the type and let the template work
#define READ(type) read(file, (type *)file)

#define READVAR(name, type) \
	type name; \
	io->read(file, (type *)&name, (String)#name)
#define READTOVAR(name, type) io->read(file, (type *)&name, (String)#name)
#define READCHECK(type, value) { \
	READVAR(check, type); \
	if (check != value)\
		return;\
}

#define READLOOPVAR(countvar, listvar, type) \
READTOVAR(countvar, U32); \
listvar = new type[countvar]; \
for (U32 i = 0; i < countvar; i ++)

#define READLISTVAR(countvar, listvar, type) \
READTOVAR(countvar, U32); \
listvar = new type[countvar]; \
for (U32 i = 0; i < countvar; i ++) { \
	READTOVAR(listvar[i], type); \
}

#define READLOOP(name) \
READVAR(name##_length, U32); \
for (U32 i = 0; i < name##_length; i ++)

#define READLIST(name, type) \
READVAR(name##_length, U32); \
for (U32 i = 0; i < name##_length; i ++) { \
	READ(type); \
}

#define READLOOPVAR2(countvar, listvar, type) \
bool read##countvar##2 = false; \
U8 read##countvar##param = 0; \
READTOVAR(countvar, U32); \
if (countvar  & 0x80000000) { \
	countvar ^= 0x80000000; \
	read##countvar##2 = true; \
	READTOVAR(read##countvar##param, U8); \
} \
listvar = new type[countvar]; \
for (U32 i = 0; i < countvar; i ++)

#define READLISTVAR2(countvar, listvar, condition, normaltype, alternatetype) \
bool read##countvar##2 = false; \
U8 read##countvar##param = 0; \
READTOVAR(countvar, U32); \
if (countvar  & 0x80000000) { \
	countvar ^= 0x80000000; \
	read##countvar##2 = true; \
	READTOVAR(read##countvar##param, U8); \
} \
listvar = new normaltype[countvar]; \
for (U32 i = 0; i < countvar; i ++) { \
	if ((condition)) { \
		READTOVAR(listvar[i], alternatetype); \
	} else { \
		READTOVAR(listvar[i], normaltype); \
	} \
}

#define READLOOP2(name) \
bool read##name##2 = false; \
U8 read##name##param = 0; \
READVAR(name##_length, U32); \
if (name##_length  & 0x80000000) { \
	name##_length ^= 0x80000000; \
	read##name##2 = true; \
	READTOVAR(read##name##param, U8); \
} \
for (U32 i = 0; i < name##_length; i ++)

#define READLIST2(name, condition, normaltype, alternatetype) \
bool read##name##2 = false; \
U8 read##name##param = 0; \
READVAR(name##_length, U32); \
if (name##_length  & 0x80000000) { \
	name##_length ^= 0x80000000; \
	read##name##2 = true; \
	READTOVAR(read##name##param, U8); \
} \
for (U32 i = 0; i < name##_length; i ++) { \
	if ((condition)) { \
		READ(alternatetype); \
	} else { \
		READ(normaltype); \
	} \
}

//Macros to speed up file reading
#define WRITE(type, value) io->write(file, value)
#define WRITECHECK(type, value) { if (WRITE(type, value)) return 0; }

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

#endif
