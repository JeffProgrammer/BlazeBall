//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef mngsupport_h
#define mngsupport_h

#include <stdbool.h>
#include "base/types.h"
#include "render/texture/bitmapTexture.h"

/**
 Reads an image from a given file using MNG (PNG/MNG support)
 @param file - The file to read from
 @param bitmap - The address of a U8* to read pixel data into
 @param dims - The address of a glm::ivec2 to read image dimensions into
 @param format - A reference to a var that will be filled with the image's format
 @return Whether or not the operation succeeded
 */
bool mngReadImage(const std::string &file, U8 *&bitmap, glm::ivec2 &dims, BitmapTexture::Format &format);

#endif
