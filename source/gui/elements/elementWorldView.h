//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GUI_ELEMENTS_ELEMENTWORLDVIEW_H_
#define _GUI_ELEMENTS_ELEMENTWORLDVIEW_H_

#include <Rocket/Core/Element.h>

class ElementWorldView : public Rocket::Core::Element {
public:
	ElementWorldView(const std::string &tag);
	~ElementWorldView();

	virtual void OnRender() override;
};

#endif // _GUI_ELEMENTS_ELEMENTWORLDVIEW_H_