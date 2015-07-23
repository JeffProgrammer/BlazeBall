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

#include "bitmap/jpegsupport.h"
#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>

bool jpegReadImage(std::string file, U8 **bitmap, Point2I *dims) {
	struct jpeg_decompress_struct dptr;
	struct jpeg_error_mgr errmgr;

	FILE *stream = fopen(file.c_str(), "r");

	dptr.err = jpeg_std_error(&errmgr);

	jpeg_create_decompress(&dptr);
	jpeg_stdio_src(&dptr, stream);

	jpeg_read_header(&dptr, TRUE);
	jpeg_start_decompress(&dptr);
	dims->x = dptr.output_width;
	dims->y = dptr.output_height;

	if (dims->x == 0 || dims->y == 0) {
		*bitmap = NULL;
		return false;
	}

	*bitmap = new U8[dims->x * dims->y * 4];

	U8 *b = *bitmap;

	for (U32 y = 0; y < dims->y; y ++) {
		JSAMPROW row = new U8[dims->x * 4];
		jpeg_read_scanlines(&dptr, &row, 1);

		for (U32 x = 0; x < dims->x; x ++) {
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
	}

	//Much more civil than MNG, 100% fewer callbacks
	jpeg_finish_decompress(&dptr);
//	jpeg_destroy_decompress(&dptr);

	return true;
}

