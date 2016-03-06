//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _RESOURCE_MATERIALRESOURCE_H_
#define _RESOURCE_MATERIALRESOURCE_H_

#include "resource/IResource.h"
#include "render/material.h"

class MaterialResource : public IResource {
	typedef IResource Parent;
public:
	MaterialResource(const std::string &file);
	~MaterialResource();

	bool load();

private:
	std::string mResourceFile;
};

#endif // _RESOURCE_MATERIALRESOURCE_H_
