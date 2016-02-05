//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
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

