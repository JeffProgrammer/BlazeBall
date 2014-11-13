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

#include <stdio.h>
#include "interior.h"
#include "math.h"

void interior_render(Interior *interior) {
	//Actual rendering is here (GL 1.1 in a 2.1 context. Take THAT, good practice!)

	Texture *currentTexture = NULL;

	//Draw all the surfaces
	for (U32 i = 0; i < interior->numSurfaces; i ++) {
		Surface surface = interior->surface[i];

		Texture *texture = interior->texture[surface.textureIndex];
		//Make sure our texture is active before drawing
		if (texture != currentTexture) {
			if (currentTexture)
				texture_deactivate(currentTexture);

			//Generate if needed
			if (!texture->generated)
				texture_generate_buffer(texture);
			texture_activate(texture);

			currentTexture = texture;
		}

		//New and improved rendering with actual Triangle Strips this time
		glBegin(GL_TRIANGLE_STRIP);
		for (U32 j = surface.windingStart; j < surface.windingStart + surface.windingCount; j ++) {
			Point3F u = interior->point[interior->index[j]];
			Point3F n = interior->normal[interior->plane[surface.planeIndex].normalIndex];

			TexGenEq texGenEq = interior->texGenEq[surface.texGenIndex];

			//Reference: TGE InteriorRender.cc
			F32 s = planeF_distance_to_point(texGenEq.planeX, u);
			F32 t = planeF_distance_to_point(texGenEq.planeY, u);
			glTexCoord2f(s, t);

			//Torque swaps y/z (x is inverted, but I can't flip without issues with normals)
			glVertex3f(u.x, u.z, u.y);
			glNormal3f(n.x, n.z, n.y);
		}
		glEnd();
	}

	//Don't forget to deactivate the last texture
	texture_deactivate(currentTexture);
}
