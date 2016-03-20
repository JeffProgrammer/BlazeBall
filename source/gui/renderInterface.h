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
// 
// Some code taken from the SDL2 sample in LibRocket.
//
// Copyright (c) 2008-2010 Nuno Silva
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//------------------------------------------------------------------------------

#ifndef _GUI_RENDERINTERFACE_H_
#define _GUI_RENDERINTERFACE_H_

#include <Rocket/Core/RenderInterface.h>
#include "platform/platformWindow.h"
#include "platform/platformGL.h"
#include "render/shader.h"

class Client;
class GuiRenderInterface : public Rocket::Core::RenderInterface {
private:
	Client *mClient;
	PlatformWindow *mWindow;
	Shader *mShader;

	GLuint mVBO;
	GLuint mIBO;
public:
	GuiRenderInterface(Client *client, PlatformWindow *window);
	~GuiRenderInterface();

	/// Called by Rocket when it wants to render geometry that it does not wish to optimise.
	virtual void RenderGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f& translation);

	/// Called by Rocket when it wants to enable or disable scissoring to clip content.
	virtual void EnableScissorRegion(bool enable);
	/// Called by Rocket when it wants to change the scissor region.
	virtual void SetScissorRegion(int x, int y, int width, int height);

	/// Called by Rocket when a texture is required by the library.
	virtual bool LoadTexture(Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source);
	/// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
	virtual bool GenerateTexture(Rocket::Core::TextureHandle& texture_handle, const Rocket::Core::byte* source, const Rocket::Core::Vector2i& source_dimensions);
	/// Called by Rocket when a loaded texture is no longer required.
	virtual void ReleaseTexture(Rocket::Core::TextureHandle texture_handle);
};

#endif //_GUI_RENDERINTERFACE_H_