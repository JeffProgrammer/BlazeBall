//------------------------------------------------------------------------------
// Copyright (c) 2014 Glenn Smith
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
#include "interior.h"
#include "math.h"

void Interior::render(Point3F offset) {
	//Actual rendering is here (GL 1.1 in a 2.1 context. Take THAT, good practice!)

	glEnable(GL_TEXTURE_2D);
	Texture *currentTexture = NULL;

	//Draw all the surfaces
	glBegin(GL_TRIANGLES);
	for (U32 i = 0; i < numSurfaces; i ++) {
		Surface surface = this->surface[i];

		Texture *texture = this->texture[surface.textureIndex];
		//Make sure our texture is active before drawing
		if (texture && texture != currentTexture) {
			glEnd();
			if (currentTexture)
				currentTexture->deactivate();

			//Generate if needed
			if (!texture->generated) {
				texture->generateBuffer();
			}
			texture->activate();

			currentTexture = texture;
			glBegin(GL_TRIANGLES);
		}

		//New and improved rendering with actual Triangle Strips this time
		for (U32 j = surface.windingStart + 2; j < surface.windingStart + surface.windingCount; j ++) {
			Point3F n = normal[plane[surface.planeIndex].normalIndex];
			if (surface.planeFlipped) {
				n *= -1;
			}
			Point3F u0, u1, u2;

			if ((j - (surface.windingStart + 2)) % 2 == 0) {
				u0 = point[index[j]];
				u1 = point[index[j - 1]];
				u2 = point[index[j - 2]];
			} else {
				u0 = point[index[j - 2]];
				u1 = point[index[j - 1]];
				u2 = point[index[j]];
			}

			TexGenEq texGenEq = this->texGenEq[surface.texGenIndex];

			//Reference: TGE InteriorRender.cc
			glNormal3f(n.x, n.y, n.z);
			glTexCoord2f(planeF_distance_to_point(texGenEq.planeX, u0), planeF_distance_to_point(texGenEq.planeY, u0));
			glVertex3f(u0.x + offset.x, u0.y + offset.y, u0.z + offset.z);

			glTexCoord2f(planeF_distance_to_point(texGenEq.planeX, u1), planeF_distance_to_point(texGenEq.planeY, u1));
			glVertex3f(u1.x + offset.x, u1.y + offset.y, u1.z + offset.z);

			glTexCoord2f(planeF_distance_to_point(texGenEq.planeX, u2), planeF_distance_to_point(texGenEq.planeY, u2));
			glVertex3f(u2.x + offset.x, u2.y + offset.y, u2.z + offset.z);
		}
	}
	glEnd();

	//Don't forget to deactivate the last texture
	if (currentTexture)
		currentTexture->deactivate();

	glDisable(GL_TEXTURE_2D);
}
