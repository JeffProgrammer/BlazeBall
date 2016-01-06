//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <mutex>
#include "base/io.h"

#ifdef _WIN32
#include "asprintf.h"
#else
#include <unistd.h>
#endif

#include "render/bitmap/mngsupport.h"
#include "render/bitmap/jpegsupport.h"
#include "render/texture/bitmapTexture.h"

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

void IO::printf(const char *format, ...) {
	static std::mutex mtx;
	{
		// Only 1 thread can access this function at a time :)
		std::lock_guard<std::mutex> guard(mtx);

		va_list args;
		va_start(args, format);
		char *buffer;
		if (vasprintf(&buffer, format, args) >= 0) {
			std::printf("%s", buffer);
			free(buffer);
		}
		va_end(args);
	}
}