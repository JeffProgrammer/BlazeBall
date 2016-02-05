//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
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

