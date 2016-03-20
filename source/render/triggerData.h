//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the project nor the
//      names of its contributors may be used to endorse or promote products
//      derived from this software without specific prior written permission.
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

#ifndef _RENDER_TRIGGERDATA_H_
#define _RENDER_TRIGGERDATA_H_

namespace TriggerData {

const static GLfloat lineVertices[] = {
	//x
		//-z
		-0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f,
		-0.5f, -0.4f, -0.5f, 0.5f, -0.4f, -0.5f,
		-0.5f, -0.3f, -0.5f, 0.5f, -0.3f, -0.5f,
		-0.5f, -0.2f, -0.5f, 0.5f, -0.2f, -0.5f,
		-0.5f, -0.1f, -0.5f, 0.5f, -0.1f, -0.5f,
		-0.5f,  0.0f, -0.5f, 0.5f,  0.0f, -0.5f,
		-0.5f,  0.1f, -0.5f, 0.5f,  0.1f, -0.5f,
		-0.5f,  0.2f, -0.5f, 0.5f,  0.2f, -0.5f,
		-0.5f,  0.3f, -0.5f, 0.5f,  0.3f, -0.5f,
		-0.5f,  0.4f, -0.5f, 0.5f,  0.4f, -0.5f,
		-0.5f,  0.5f, -0.5f, 0.5f,  0.5f, -0.5f,
		//+z
		-0.5f, -0.5f,  0.5f, 0.5f, -0.5f,  0.5f,
		-0.5f, -0.4f,  0.5f, 0.5f, -0.4f,  0.5f,
		-0.5f, -0.3f,  0.5f, 0.5f, -0.3f,  0.5f,
		-0.5f, -0.2f,  0.5f, 0.5f, -0.2f,  0.5f,
		-0.5f, -0.1f,  0.5f, 0.5f, -0.1f,  0.5f,
		-0.5f,  0.0f,  0.5f, 0.5f,  0.0f,  0.5f,
		-0.5f,  0.1f,  0.5f, 0.5f,  0.1f,  0.5f,
		-0.5f,  0.2f,  0.5f, 0.5f,  0.2f,  0.5f,
		-0.5f,  0.3f,  0.5f, 0.5f,  0.3f,  0.5f,
		-0.5f,  0.4f,  0.5f, 0.5f,  0.4f,  0.5f,
		-0.5f,  0.5f,  0.5f, 0.5f,  0.5f,  0.5f,
		//-y
		-0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.4f, 0.5f, -0.5f, -0.4f,
		-0.5f, -0.5f, -0.3f, 0.5f, -0.5f, -0.3f,
		-0.5f, -0.5f, -0.2f, 0.5f, -0.5f, -0.2f,
		-0.5f, -0.5f, -0.1f, 0.5f, -0.5f, -0.1f,
		-0.5f, -0.5f,  0.0f, 0.5f, -0.5f,  0.0f,
		-0.5f, -0.5f,  0.1f, 0.5f, -0.5f,  0.1f,
		-0.5f, -0.5f,  0.2f, 0.5f, -0.5f,  0.2f,
		-0.5f, -0.5f,  0.3f, 0.5f, -0.5f,  0.3f,
		-0.5f, -0.5f,  0.4f, 0.5f, -0.5f,  0.4f,
		-0.5f, -0.5f,  0.5f, 0.5f, -0.5f,  0.5f,
		//+y
		-0.5f,  0.5f, -0.5f, 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.4f, 0.5f,  0.5f, -0.4f,
		-0.5f,  0.5f, -0.3f, 0.5f,  0.5f, -0.3f,
		-0.5f,  0.5f, -0.2f, 0.5f,  0.5f, -0.2f,
		-0.5f,  0.5f, -0.1f, 0.5f,  0.5f, -0.1f,
		-0.5f,  0.5f,  0.0f, 0.5f,  0.5f,  0.0f,
		-0.5f,  0.5f,  0.1f, 0.5f,  0.5f,  0.1f,
		-0.5f,  0.5f,  0.2f, 0.5f,  0.5f,  0.2f,
		-0.5f,  0.5f,  0.3f, 0.5f,  0.5f,  0.3f,
		-0.5f,  0.5f,  0.4f, 0.5f,  0.5f,  0.4f,
		-0.5f,  0.5f,  0.5f, 0.5f,  0.5f,  0.5f,

	//y
		//-z
		-0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f,
		-0.4f, -0.5f, -0.5f, -0.4f, 0.5f, -0.5f,
		-0.3f, -0.5f, -0.5f, -0.3f, 0.5f, -0.5f,
		-0.2f, -0.5f, -0.5f, -0.2f, 0.5f, -0.5f,
		-0.1f, -0.5f, -0.5f, -0.1f, 0.5f, -0.5f,
		 0.0f, -0.5f, -0.5f,  0.0f, 0.5f, -0.5f,
		 0.1f, -0.5f, -0.5f,  0.1f, 0.5f, -0.5f,
		 0.2f, -0.5f, -0.5f,  0.2f, 0.5f, -0.5f,
		 0.3f, -0.5f, -0.5f,  0.3f, 0.5f, -0.5f,
		 0.4f, -0.5f, -0.5f,  0.4f, 0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,  0.5f, 0.5f, -0.5f,
		//+z
		-0.5f, -0.5f,  0.5f, -0.5f, 0.5f,  0.5f,
		-0.4f, -0.5f,  0.5f, -0.4f, 0.5f,  0.5f,
		-0.3f, -0.5f,  0.5f, -0.3f, 0.5f,  0.5f,
		-0.2f, -0.5f,  0.5f, -0.2f, 0.5f,  0.5f,
		-0.1f, -0.5f,  0.5f, -0.1f, 0.5f,  0.5f,
		 0.0f, -0.5f,  0.5f,  0.0f, 0.5f,  0.5f,
		 0.1f, -0.5f,  0.5f,  0.1f, 0.5f,  0.5f,
		 0.2f, -0.5f,  0.5f,  0.2f, 0.5f,  0.5f,
		 0.3f, -0.5f,  0.5f,  0.3f, 0.5f,  0.5f,
		 0.4f, -0.5f,  0.5f,  0.4f, 0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,  0.5f, 0.5f,  0.5f,
		//-x
		-0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f,
		-0.5f, -0.5f, -0.4f, -0.5f, 0.5f, -0.4f,
		-0.5f, -0.5f, -0.3f, -0.5f, 0.5f, -0.3f,
		-0.5f, -0.5f, -0.2f, -0.5f, 0.5f, -0.2f,
		-0.5f, -0.5f, -0.1f, -0.5f, 0.5f, -0.1f,
		-0.5f, -0.5f,  0.0f, -0.5f, 0.5f,  0.0f,
		-0.5f, -0.5f,  0.1f, -0.5f, 0.5f,  0.1f,
		-0.5f, -0.5f,  0.2f, -0.5f, 0.5f,  0.2f,
		-0.5f, -0.5f,  0.3f, -0.5f, 0.5f,  0.3f,
		-0.5f, -0.5f,  0.4f, -0.5f, 0.5f,  0.4f,
		-0.5f, -0.5f,  0.5f, -0.5f, 0.5f,  0.5f,
		//+x
		 0.5f, -0.5f, -0.5f,  0.5f, 0.5f, -0.5f,
		 0.5f, -0.5f, -0.4f,  0.5f, 0.5f, -0.4f,
		 0.5f, -0.5f, -0.3f,  0.5f, 0.5f, -0.3f,
		 0.5f, -0.5f, -0.2f,  0.5f, 0.5f, -0.2f,
		 0.5f, -0.5f, -0.1f,  0.5f, 0.5f, -0.1f,
		 0.5f, -0.5f,  0.0f,  0.5f, 0.5f,  0.0f,
		 0.5f, -0.5f,  0.1f,  0.5f, 0.5f,  0.1f,
		 0.5f, -0.5f,  0.2f,  0.5f, 0.5f,  0.2f,
		 0.5f, -0.5f,  0.3f,  0.5f, 0.5f,  0.3f,
		 0.5f, -0.5f,  0.4f,  0.5f, 0.5f,  0.4f,
		 0.5f, -0.5f,  0.5f,  0.5f, 0.5f,  0.5f,

	//z
		//-y
		-0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,
		-0.4f, -0.5f, -0.5f, -0.4f, -0.5f, 0.5f,
		-0.3f, -0.5f, -0.5f, -0.3f, -0.5f, 0.5f,
		-0.2f, -0.5f, -0.5f, -0.2f, -0.5f, 0.5f,
		-0.1f, -0.5f, -0.5f, -0.1f, -0.5f, 0.5f,
		 0.0f, -0.5f, -0.5f,  0.0f, -0.5f, 0.5f,
		 0.1f, -0.5f, -0.5f,  0.1f, -0.5f, 0.5f,
		 0.2f, -0.5f, -0.5f,  0.2f, -0.5f, 0.5f,
		 0.3f, -0.5f, -0.5f,  0.3f, -0.5f, 0.5f,
		 0.4f, -0.5f, -0.5f,  0.4f, -0.5f, 0.5f,
		 0.5f, -0.5f, -0.5f,  0.5f, -0.5f, 0.5f,
		//+y
		-0.5f,  0.5f, -0.5f, -0.5f,  0.5f, 0.5f,
		-0.4f,  0.5f, -0.5f, -0.4f,  0.5f, 0.5f,
		-0.3f,  0.5f, -0.5f, -0.3f,  0.5f, 0.5f,
		-0.2f,  0.5f, -0.5f, -0.2f,  0.5f, 0.5f,
		-0.1f,  0.5f, -0.5f, -0.1f,  0.5f, 0.5f,
		 0.0f,  0.5f, -0.5f,  0.0f,  0.5f, 0.5f,
		 0.1f,  0.5f, -0.5f,  0.1f,  0.5f, 0.5f,
		 0.2f,  0.5f, -0.5f,  0.2f,  0.5f, 0.5f,
		 0.3f,  0.5f, -0.5f,  0.3f,  0.5f, 0.5f,
		 0.4f,  0.5f, -0.5f,  0.4f,  0.5f, 0.5f,
		 0.5f,  0.5f, -0.5f,  0.5f,  0.5f, 0.5f,
		//-x
		-0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,
		-0.5f, -0.4f, -0.5f, -0.5f, -0.4f, 0.5f,
		-0.5f, -0.3f, -0.5f, -0.5f, -0.3f, 0.5f,
		-0.5f, -0.2f, -0.5f, -0.5f, -0.2f, 0.5f,
		-0.5f, -0.1f, -0.5f, -0.5f, -0.1f, 0.5f,
		-0.5f,  0.0f, -0.5f, -0.5f,  0.0f, 0.5f,
		-0.5f,  0.1f, -0.5f, -0.5f,  0.1f, 0.5f,
		-0.5f,  0.2f, -0.5f, -0.5f,  0.2f, 0.5f,
		-0.5f,  0.3f, -0.5f, -0.5f,  0.3f, 0.5f,
		-0.5f,  0.4f, -0.5f, -0.5f,  0.4f, 0.5f,
		-0.5f,  0.5f, -0.5f, -0.5f,  0.5f, 0.5f,
		//+x
		 0.5f, -0.5f, -0.5f,  0.5f, -0.5f, 0.5f,
		 0.5f, -0.4f, -0.5f,  0.5f, -0.4f, 0.5f,
		 0.5f, -0.3f, -0.5f,  0.5f, -0.3f, 0.5f,
		 0.5f, -0.2f, -0.5f,  0.5f, -0.2f, 0.5f,
		 0.5f, -0.1f, -0.5f,  0.5f, -0.1f, 0.5f,
		 0.5f,  0.0f, -0.5f,  0.5f,  0.0f, 0.5f,
		 0.5f,  0.1f, -0.5f,  0.5f,  0.1f, 0.5f,
		 0.5f,  0.2f, -0.5f,  0.5f,  0.2f, 0.5f,
		 0.5f,  0.3f, -0.5f,  0.5f,  0.3f, 0.5f,
		 0.5f,  0.4f, -0.5f,  0.5f,  0.4f, 0.5f,
		 0.5f,  0.5f, -0.5f,  0.5f,  0.5f, 0.5f
};

//Vertices shamelessly stolen from
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-4-a-colored-cube/
const static GLfloat cubeVertices[] = {
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f
};

}

#endif
