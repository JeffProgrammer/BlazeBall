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

#ifndef jpegsupport_h
#define jpegsupport_h

#include <stdbool.h>
#include "types.h"

/**
 Reads an image from a given file using JPEG
 @param file - The file to read from
 @param bitmap - The address of a U8* to read pixel data into
 @param dims - The address of a Point2I to read image dimensions into
 @return Whether or not the operation succeeded
 */
bool jpegReadImage(String file, U8 **bitmap, Point2I *dims);

#endif
