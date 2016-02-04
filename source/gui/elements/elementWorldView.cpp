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
	glm::ivec2 size(GetClientWidth(), GetClientHeight());

	//Optimize
	if (size.x == 0 || size.y == 0)
		return;
	glEnable(GL_SCISSOR_TEST);

	RenderInfo info = mClient->getRenderer()->getRenderInfo(size);
	info.viewport.size = size;
	info.viewport.position = glm::ivec2(GetAbsoluteLeft(), GetAbsoluteTop());

	info.setViewport();
	info.setScissor();

	static_cast<RenderWorld *>(mClient->getWorld())->render(info);

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

