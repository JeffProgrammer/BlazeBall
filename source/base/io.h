//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the project nor the
//      names of its contributors may be used to endorse or promote products
//      derived from this software without specific prior written permission.
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

#ifndef _BASE_IO_H_
#define _BASE_IO_H_

//#ifdef _WIN32
//#define DIR_SEP '\\'
//#else
#define DIR_SEP '/'
//#endif

#include <string>
#include "base/types.h"
#include "base/math.h"
#include "render/texture/texture.h"

class IO {
public:
	/**
	 * Get the path of a file up to but not including the file name
	 * @param file The file whose path to get
	 * @param separator The directory separator to use (platform-specific by default)
	 * @return The file's path
	 */
	static const std::string getPath(const std::string &file, const char &separator = DIR_SEP);
	/**
	 * Get the name of a file inluding the directory but not the path
	 * @param file The file whose name to get
	 * @param separator The directory separator to use (platform-specific by default)
	 * @return The file's name
	 */
	static const std::string getName(const std::string &file, const char &separator = DIR_SEP);
	/**
	 * Get the extension of a file
	 * @param file The file whose extension to get
	 * @return The file's extension
	 */
	static const std::string getExtension(const std::string &file);
	/**
	 * Get the base name of a file including neither directory nor path
	 * @param file The file whose base name to get
	 * @param separator The directory separator to use (platform-specific by default)
	 * @return The file's base name
	 */
	static const std::string getBase(const std::string &file, const char &separator = DIR_SEP);
	
	/**
	 * Determine if a file exists in the filesystem.
	 * @param file The file whose existence to check
	 * @return If the file exists
	 */
	static bool isfile(const std::string &file);
	/**
	 * Read a file from the filesystem in its entirety to a U8* buffer.
	 * @param file The file to read
	 * @param length A U32 in which the file's length will be stored
	 * @return A character buffer containing the contents of the file, or NULL if
	 *         the file could not be found.
	 */
	static U8 *readFile(const std::string &file, U32 &length);
	
	static Texture *loadTexture(const std::string &file);

	static void printf(const char *format, ...);
};

#endif // _BASE_IO_H_