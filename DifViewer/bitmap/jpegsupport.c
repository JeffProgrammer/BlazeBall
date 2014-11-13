//------------------------------------------------------------------------------
// Copyright (C) 2014 Glenn Smith
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//------------------------------------------------------------------------------

#include "jpegsupport.h"
#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>

bool jpegReadImage(String file, U8 **bitmap, Point2I *dims) {
	struct jpeg_decompress_struct dptr;
	struct jpeg_error_mgr errmgr;

	FILE *stream = fopen((const char *)file, "r");

	dptr.err = jpeg_std_error(&errmgr);

	jpeg_create_decompress(&dptr);
	jpeg_stdio_src(&dptr, stream);

	jpeg_read_header(&dptr, true);
	jpeg_start_decompress(&dptr);
	dims->x = dptr.output_width;
	dims->y = dptr.output_height;

	if (dims->x == 0 || dims->y == 0) {
		*bitmap = NULL;
		return false;
	}

	*bitmap = malloc(sizeof(U8) * dims->x * dims->y * 4);

	U8 *b = *bitmap;

	for (U32 y = 0; y < dims->y; y ++) {
		JSAMPROW row = calloc(dims->x, sizeof(U8) * 4);
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
	jpeg_destroy_decompress(&dptr);

	return true;
}

