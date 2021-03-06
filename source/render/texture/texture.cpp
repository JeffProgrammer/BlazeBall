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

#include "texture.h"
#include "base/io.h"

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