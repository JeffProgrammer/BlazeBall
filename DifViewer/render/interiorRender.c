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

void interior_render(Interior *interior, Point3F offset) {
	//Actual rendering is here (GL 1.1 in a 2.1 context. Take THAT, good practice!)

	Texture *currentTexture = NULL;

	//Draw all the surfaces
	for (U32 i = 0; i < interior->numSurfaces; i ++) {
		Surface surface = interior->surface[i];

		Texture *texture = interior->texture[surface.textureIndex];
		//Make sure our texture is active before drawing
		if (texture && texture != currentTexture) {
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
			glVertex3f(u.x + offset.x, u.z + offset.z, u.y + offset.y);
			glNormal3f(n.x, n.z, n.y);
		}
		glEnd();
	}

	//Don't forget to deactivate the last texture
	texture_deactivate(currentTexture);
}
