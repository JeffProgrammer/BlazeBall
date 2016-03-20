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

#include "render/bitmap/tgasupport.h"
#include <stdio.h>
#include <stdlib.h>
#include <tga.h>
#include "base/io.h"

bool tgaReadImage(const std::string &file, U8 *&bitmap, glm::ivec2 &dims, BitmapTexture::Format &format) {
	tgaInfo *info = tgaLoad(file.c_str());

	if (info == NULL) {
		return false;
	}
	if (info->status != TGA_OK) {
		tgaDestroy(info);
		IO::printf("Error reading TGA: %d", info->status);
		return false;
	}

	dims.x = info->width;
	dims.y = info->height;
	format = (info->pixelDepth == 24 ? BitmapTexture::FormatRGB8 : BitmapTexture::FormatRGBA8);

	bitmap = new U8[info->width * info->height * (info->pixelDepth / 8)];
	memcpy(bitmap, info->imageData, info->width * info->height * (info->pixelDepth / 8));

	tgaDestroy(info);

	return true;
}

