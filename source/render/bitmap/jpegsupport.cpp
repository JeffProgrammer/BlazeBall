//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "render/bitmap/jpegsupport.h"
#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>

bool jpegReadImage(const std::string &file, U8 *&bitmap, glm::ivec2 &dims, BitmapTexture::Format &format) {
	struct jpeg_decompress_struct dptr;
	struct jpeg_error_mgr errmgr;

	FILE *stream = fopen(file.c_str(), "rb");

	dptr.err = jpeg_std_error(&errmgr);

	jpeg_create_decompress(&dptr);
	jpeg_stdio_src(&dptr, stream);

	jpeg_read_header(&dptr, TRUE);
	jpeg_start_decompress(&dptr);
	dims.x = dptr.output_width;
	dims.y = dptr.output_height;

	if (dims.x == 0 || dims.y == 0) {
		bitmap = NULL;
		return false;
	}

	format = BitmapTexture::FormatRGBA8;
	bitmap = new U8[dims.x * dims.y * 4];

	U8 *b = bitmap;

	for (S32 y = 0; y < dims.y; y ++) {
		JSAMPROW row = new U8[dims.x * 4];
		jpeg_read_scanlines(&dptr, &row, 1);

		for (S32 x = 0; x < dims.x; x ++) {
			switch (dptr.output_components) {
				case 1: //Grayscale
					*b++ = row[x];
					*b++ = row[x];
					*b++ = row[x];
					*b++ = 255;
					break;
				case 3: //RGB
					*b++ = row[(x * 3)];
					*b++ = row[(x * 3) + 1];
					*b++ = row[(x * 3) + 2];
					*b++ = 255;
					break;
				default:
					break;
			}
		}

		delete [] row;
	}

	//Much more civil than MNG, 100% fewer callbacks
	jpeg_finish_decompress(&dptr);
	jpeg_destroy_decompress(&dptr);

	fclose(stream);

	return true;
}

