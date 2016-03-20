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

#include "gui/elements/elementWorldView.h"
#include "network/client.h"
#include "render/renderWorld.h"
#include "render/renderer.h"

ElementWorldView::ElementWorldView(const Rocket::Core::String &tag, Client *client) : Rocket::Core::Element(tag), mClient(client) {

}

ElementWorldView::~ElementWorldView() {

}

void ElementWorldView::OnRender() {
	//Get extent from the DOM
	glm::ivec2 size(GetClientWidth(), GetClientHeight());

	//Optimize
	if (size.x == 0 || size.y == 0)
		return;
	glEnable(GL_SCISSOR_TEST);

	//Get a RenderInfo so we can render the world
	RenderInfo info = mClient->getRenderer()->getRenderInfo(size);

	//Update viewport settings because we might be offset somewhat
	info.viewport.size = size;
	info.viewport.position = glm::ivec2(GetAbsoluteLeft(), GetAbsoluteTop());

	//Focus on our region
	info.setViewport();
	info.setScissor();

	//Actually render
	static_cast<RenderWorld *>(mClient->getWorld())->render(info);

	//Disable scissor for now
	glDisable(GL_SCISSOR_TEST);
}

ElementInstancerWorldView::ElementInstancerWorldView(Client *client) : mClient(client) {

}

Rocket::Core::Element *ElementInstancerWorldView::InstanceElement(Rocket::Core::Element *parent, const Rocket::Core::String &tag, const Rocket::Core::XMLAttributes &attributes) {
	//Little bit of dependency injection here
	return new ElementWorldView(tag, mClient);
}
void ElementInstancerWorldView::ReleaseElement(Rocket::Core::Element *element) {
	delete element;
}
void ElementInstancerWorldView::Release() {
	delete this;
}

