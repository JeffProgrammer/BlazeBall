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
#include <stdbool.h>
#include "bitmap/mngsupport.h"
#include "base/types.h"
#include <mng/libmng.h>

typedef enum {
	BitmapFormatRGB8 = 3,
	BitmapFormatRGBA8 = 4
} BitmapFormat;

typedef struct {
	bool inited;
	std::string file;
	FILE *stream;
	glm::ivec2 extent;
	U8 *pixels;
	BitmapFormat format;
} MNGInfo;

static mng_handle gMNG = MNG_NULL;
static MNGInfo gMNGInfo;

bool initMNG();
void closeMNG();

mng_ptr mng__memalloc(mng_size_t len) {
	//Must be zeroed
	return calloc(1, len);
}

void mng__memfree(mng_ptr ptr, mng_size_t len) {
	free(ptr);
}

mng_bool mng__openstream(mng_handle handle) {
	//Init info
	MNGInfo *info = (MNGInfo *)mng_get_userdata(handle);
	info->extent.x = 0;
	info->extent.y = 0;

	return MNG_TRUE;
}

mng_bool mng__closestream(mng_handle handle) {
	return MNG_TRUE;
}

mng_bool mng__readdata(mng_handle handle, mng_ptr data, mng_uint32 length, mng_uint32p bytesread) {
	MNGInfo *info = (MNGInfo *)mng_get_userdata(handle);
	if (info->stream == NULL) {
		return false;
	}
	//Read data
	U32 read = (U32)fread(data, 1, length, info->stream);
	*bytesread = read;

	return read > 0;
}

mng_bool mng__processheader(mng_handle handle, mng_uint32 width, mng_uint32 height) {
	MNGInfo *info = (MNGInfo *)mng_get_userdata(handle);

	//Set extent
	info->extent.x = width;
	info->extent.y = height;

	mng_uint8 colorType = mng_get_colortype(handle);
	mng_uint8 alphaDepth = mng_get_alphadepth(handle);

	switch (colorType) {
		case MNG_COLORTYPE_GRAY:
		case MNG_COLORTYPE_JPEGGRAY:
			//No alpha
			info->format = BitmapFormatRGB8;
			mng_set_canvasstyle(handle, MNG_CANVAS_RGB8);
			break;
		case MNG_COLORTYPE_INDEXED:
		case MNG_COLORTYPE_RGB:
		case MNG_COLORTYPE_JPEGCOLOR:
			//May have alpha, not sure
			if (alphaDepth >= 1) {
				info->format = BitmapFormatRGBA8;
				mng_set_canvasstyle(handle, MNG_CANVAS_RGBA8);
			} else {
				info->format = BitmapFormatRGB8;
				mng_set_canvasstyle(handle, MNG_CANVAS_RGB8);
			}
			break;
		case MNG_COLORTYPE_RGBA:
		case MNG_COLORTYPE_JPEGCOLORA:
			//Always have alpha
			info->format = BitmapFormatRGBA8;
			mng_set_canvasstyle(handle, MNG_CANVAS_RGBA8);
			break;
		default:
			return false;
	}

	//Allocate the image
	info->pixels = new U8[width * height * info->format];

	return MNG_TRUE;
}

mng_ptr mng__getcanvasline(mng_handle handle, mng_uint32 line) {
	MNGInfo *info = (MNGInfo *)mng_get_userdata(handle);

	return info->pixels + (line * gMNGInfo.extent.x * info->format);
}

mng_bool mng__refresh(mng_handle handle, mng_uint32 x, mng_uint32 y, mng_uint32 w, mng_uint32 h) {
	return MNG_TRUE;
}

mng_uint32 mng__gettickcount(mng_handle handle) {
	return 0;
}

mng_bool mng__settimer(mng_handle handle, mng_uint32 msecs) {
	return MNG_TRUE;
}

bool mngReadImage(const std::string &file, U8 *&bitmap, glm::ivec2 &dims) {
	if (!initMNG()) {
		bitmap = NULL;
		return false;
	}

	gMNGInfo.file = file;
	gMNGInfo.pixels = bitmap;
	gMNGInfo.stream = fopen(file.c_str(), "r");

	if (mng_set_suspensionmode(gMNG, MNG_FALSE) != MNG_NOERROR) {
		mng_reset(gMNG);
		bitmap = NULL;
		return false;
	}

	mng_retcode status = mng_read(gMNG);
	while (status == MNG_NEEDMOREDATA) {
		status = mng_read_resume(gMNG);
	}

	if (status != MNG_NOERROR) {
		mng_reset(gMNG);
		bitmap = NULL;
		return false;
	}

	status = mng_display(gMNG);

	while (status == MNG_NEEDTIMERWAIT) {
		status = mng_display_resume(gMNG);
	}
	if (status != MNG_NOERROR) {
		mng_reset(gMNG);
		bitmap = NULL;
		return false;
	}

	mng_reset(gMNG);

	dims.x = gMNGInfo.extent.x;
	dims.y = gMNGInfo.extent.y;

	closeMNG();

	return true;
}

bool initMNG() {
	gMNGInfo.inited = false;
	memset(&gMNGInfo, 0, sizeof(MNGInfo));

	gMNG = mng_initialize(&gMNGInfo, mng__memalloc, mng__memfree, MNG_NULL);
	if (gMNG == MNG_NULL) {
		return false;
	}

	if (mng_setcb_openstream(gMNG, mng__openstream) != MNG_NOERROR) {
		closeMNG();
		return false;
	}

	if (mng_setcb_readdata(gMNG, mng__readdata) != MNG_NOERROR) {
		closeMNG();
		return false;
	}

	if (mng_setcb_closestream(gMNG, mng__closestream) != MNG_NOERROR) {
		closeMNG();
		return false;
	}

	if (mng_setcb_processheader(gMNG, mng__processheader) != MNG_NOERROR) {
		closeMNG();
		return false;
	}

	if (mng_setcb_getcanvasline(gMNG, mng__getcanvasline) != MNG_NOERROR) {
		closeMNG();
		return false;
	}

	if (mng_setcb_refresh(gMNG, mng__refresh) != MNG_NOERROR) {
		closeMNG();
		return false;
	}

	if (mng_setcb_gettickcount(gMNG, mng__gettickcount) != MNG_NOERROR) {
		closeMNG();
		return false;
	}

	if (mng_setcb_settimer(gMNG, mng__settimer) != MNG_NOERROR) {
		closeMNG();
		return false;
	}

	return true;
}

void closeMNG() {
	if (gMNG != MNG_NULL) {
		mng_cleanup(&gMNG);
	}
	gMNG = MNG_NULL;
	gMNGInfo.file = "";
}