//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
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
#include <string>
#include <math.h>
#include "base/io.h"
#include "render/bitmap/mngsupport.h"
#include "render/bitmap/jpegsupport.h"
#include "texture/bitmapTexture.h"

bool IO::isfile(const std::string &file) {
	FILE *stream = fopen(file.c_str(), "rb");
	if (stream) {
		fclose(stream);
		return true;
	}
	return false;
}

U8 *IO::readFile(const std::string &file, U32 &length) {
	FILE *stream = fopen(file.c_str(), "rb");

	if (!stream)
		return NULL;

	//Read length of file
	fseek(stream, 0L, SEEK_END);
	length = (U32)ftell(stream);
	fseek(stream, 0L, SEEK_SET);

	U8 *data = new U8[length + 1];
	fread(data, sizeof(U8), length, stream);
	data[length ++] = 0;

	fclose(stream);

	return data;
}

const std::string IO::getPath(const std::string &file, const char &seperator) {
	std::string::size_type last = file.find_last_of(seperator);
	if (last != std::string::npos)
		return file.substr(0, last);
	return "";
}

const std::string IO::getName(const std::string &file, const char &seperator) {
	std::string::size_type last = file.find_last_of(seperator) + 1;
	if (last != std::string::npos)
		return file.substr(last);
	return file;
}

const std::string IO::getExtension(const std::string &file) {
	std::string::size_type last = file.find_last_of('.') + 1;
	if (last != std::string::npos)
		return file.substr(last);
	return "";
}

const std::string IO::getBase(const std::string &file, const char &seperator) {
	std::string::size_type slash = file.find_last_of(seperator) + 1;
	std::string::size_type dot = file.find_last_of('.');
	if (slash != std::string::npos) {
		if (dot != std::string::npos)
			return file.substr(slash, dot - slash);
		else
			return file.substr(slash);
	}
	return file;
}

Texture *IO::loadTexture(const std::string &file) {
	U8 *bitmap;
	glm::ivec2 dims;
	BitmapTexture::Format format;
	std::string extension = getExtension(file);

	bool (*readFn)(const std::string &file, U8 *&bitmap, glm::ivec2 &dims, BitmapTexture::Format &format);

	//Try to read the image based on format
	if (extension == "png" || extension == "jng")
		readFn = mngReadImage;
	else if (extension == "jpg")
		readFn = jpegReadImage;
	else {
		// ?!
		readFn = mngReadImage;
	}

	if (readFn(file, bitmap, dims, format)) {
		Texture *tex = new BitmapTexture(bitmap, dims, format);
		delete [] bitmap;

		return tex;
	}

	return NULL;
}
