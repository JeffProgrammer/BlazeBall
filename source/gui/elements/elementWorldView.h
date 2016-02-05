//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GUI_ELEMENTS_ELEMENTWORLDVIEW_H_
#define _GUI_ELEMENTS_ELEMENTWORLDVIEW_H_

#include <Rocket/Core/Element.h>
#include <Rocket/Core/ElementInstancer.h>

class Client;

class ElementWorldView : public Rocket::Core::Element {
protected:
	Client *mClient;
public:
	ElementWorldView(const Rocket::Core::String &tag, Client *client);
	~ElementWorldView();

	virtual void OnRender() override;
};

class ElementInstancerWorldView : public Rocket::Core::ElementInstancer {
protected:
	Client *mClient;
public:
	ElementInstancerWorldView(Client *client);
	
	virtual Rocket::Core::Element *InstanceElement(Rocket::Core::Element* parent, const Rocket::Core::String &tag, const Rocket::Core::XMLAttributes &attributes) override;
	virtual void ReleaseElement(Rocket::Core::Element *element) override;
	virtual void Release() override;
};

#endif // _GUI_ELEMENTS_ELEMENTWORLDVIEW_H_