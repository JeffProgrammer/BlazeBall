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
#include <string>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#else
#include <GL/glew.h>
#endif

#include "base/io.h"
#include "render/bitmap/texture.h"
#include "graphics/gl/glUtils.h"

#define TEXTURE_MAX_SIZE 1024

Texture::Texture(U8 *pixels, const glm::ivec2 &extent, const BitmapFormat &format) {
	if (extent.x > TEXTURE_MAX_SIZE || extent.y > TEXTURE_MAX_SIZE) {
		printf("Texture too large! (%d, %d) > (%d, %d). Bug HiGuy to make textures larger.", extent.x, extent.y, TEXTURE_MAX_SIZE, TEXTURE_MAX_SIZE);
		return;
	}

	//Set some fields
	this->extent = extent;
	this->format = format;
	generated = false;

	//Load pixels into pixels (assume RGBA)
	this->pixels = new U8[extent.x * extent.y * format];
	memcpy(this->pixels, pixels, sizeof(U8) * extent.x * extent.y * format);
}

void Texture::generateBuffer() {
	//Set mode
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//Generate the texture buffer
	glGenTextures(1, &buffer);
	glBindTexture(GL_TEXTURE_2D, buffer);

	//Set some flags
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Actually create the texture
	GLenum glformat = (format == BitmapFormatRGB8 ? GL_RGB : GL_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, glformat, extent.x, extent.y, 0, glformat, GL_UNSIGNED_BYTE, pixels);

	// When MAGnifying the image (no bigger mipmap available), use LINEAR filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// Generate mipmaps, by the way.
	glGenerateMipmap(GL_TEXTURE_2D);

	// check for errors
	GL_CHECKERRORS();

	generated = true;
}

Texture::~Texture() {
	//Clean up
	if (generated)
		glDeleteTextures(1, &buffer);

	delete pixels;
}

void Texture::activate() {
	//Error check
	if (!generated)
		return;
	//Activate and bind the buffer
	glActiveTexture(texNum);
	glBindTexture(GL_TEXTURE_2D, buffer);
}

void Texture::deactivate() {
	//Haha, this method is just BS. Fooled you.
}

/**
 * Check if a texture exists at the given path that Torque can use. Textures should
 * be passed without any extension.
 * @param path The path for the texture to check
 * @param name The base name of the texture
 * @param final A string into which the final result will be stored if there was success
 * @return If the texture exists
 */
inline bool checkTexture(const std::string &path, const std::string &name, std::string &final) {
	//Check for no extension
	final = path + DIR_SEP + name;
	if (IO::isfile(final))
		return true;
	//Check for .png
	final = path + DIR_SEP + name + ".png";
	if (IO::isfile(final))
		return true;
	//Check for .jpg
	final = path + DIR_SEP + name + ".jpg";
	if (IO::isfile(final))
		return true;
	// Check for .jng
	final  =path + DIR_SEP + name + ".jng";
	if (IO::isfile(final))
		return true;
	//Couldn't find it, clean up
	final = "";
	return false;
}

/**
 * Find the closest texture to a given texture name, as Torque adds unnecessary
 * directories onto texture paths. This will attempt to find the texture in any
 * parent directory of the given texture's directory.
 * @param fullName The texture's path for which to search
 * @return The closest actually existing texture's path for the given texture, or
 *         just the texture name again if none is found.
 */
std::string Texture::find(const std::string &fullName) {
	//Search recurse directories
	std::string testName(fullName);
	//Base file name for checking
	std::string baseName =  IO::getName(fullName);
	
	//Iterate over every subdirectory in the path and check if it has the file
	while (testName.find_last_of(DIR_SEP) != std::string::npos) {
		//Strip off the last directory
		testName = IO::getPath(testName);
		
		//Check if the texture exists at the current location
		std::string finalName;
		if (checkTexture(testName, baseName, finalName)) {
			//Let us know
			return finalName;
		}
	}
	//Couldn't find it?
	return "";
}