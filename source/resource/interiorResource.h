//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _RESOURCE_INTERIORRESOURCE_H_
#define _RESOURCE_INTERIORRESOURCE_H_

#include <dif/objects/dif.h>
#include <vector>
#include "base/types.h"
#include "base/io.h"
#include "resource/IResource.h"

class InteriorResource : public IResource {
	typedef IResource Parent;
public:
	InteriorResource(const std::string &file);
	~InteriorResource();

	bool load();

private:
	std::string mResourceFile;
};

#endif // _RESOURCE_INTERIORRESOURCE_H_
