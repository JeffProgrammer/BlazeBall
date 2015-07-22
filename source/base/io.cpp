//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
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
#include "base/io.h"
#include "bitmap/mngsupport.h"
#include "bitmap/jpegsupport.h"

bool IO::isfile(const String &file) {
	FILE *stream = fopen((const char *)file.data, "r");
	if (stream) {
		fclose(stream);
		return true;
	}
	return false;
}

U8 *IO::readFile(const String &file, U32 *length) {
	FILE *stream = fopen((const char *)file.data, "rb");

	if (!stream)
		return NULL;

	//Read length of file
	fseek(stream, 0L, SEEK_END);
	*length = (U32)ftell(stream);
	fseek(stream, 0L, SEEK_SET);

	U8 *data = new U8[*length + 1];
	fread(data, sizeof(U8), *length, stream);
	data[*length ++] = 0;

	fclose(stream);

	return data;
}

#ifdef _WIN32
#define DIR_SEP '\\'
#else
#define DIR_SEP '/'
#endif


String IO::getPath(const String &file) {
	S64 last = (S64)((U8 *)strrchr((const char *)file.data, DIR_SEP) - file.data);
	if (last > 0)
		return String(file, (S32)last);
	return String("");
}
String IO::getName(const String &file) {
	S64 last = (S64)((U8 *)strrchr((const char *)file.data, DIR_SEP) - file.data) + 1;
	printf("Last is %lld\n", last);
	if (last > 0)
		return String(file.data + last, file.length - (S32)last);
	return file;
}
String IO::getExtension(const String &file) {
	S64 last = (S64)((U8 *)strrchr((const char *)file.data, '.') - file.data) + 1;
	if (last > 0)
		return String(file.data + last, file.length - (S32)last);
	return String("");
}

Texture *IO::loadTexture(const String &file) {
	U8 *bitmap;
	Point2I dims;

	String extension = getExtension(file);

	bool (*readFn)(String file, U8 **bitmap, Point2I *dims);

	//Try to read the image based on format
	if (extension == "png" || extension == "jng")
		readFn = mngReadImage;
	else if (extension == "jpg")
		readFn = jpegReadImage;
	else {
		// ?!
		readFn = mngReadImage;
	}

	if (readFn(file, &bitmap, &dims)) {
		Texture *tex = new Texture(bitmap, dims);
		delete [] bitmap;

		return tex;
	}

	return NULL;
}
