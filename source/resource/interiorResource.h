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
#include "resource/MeshResource.h"

class InteriorResource : public MeshResource {
	typedef MeshResource Parent;
public:
	InteriorResource(const std::string &file);
	~InteriorResource();

	virtual bool load() override;

	virtual const BoxF getWorldBox() const override {
		return mWorldBox;
	}

private:
	BoxF mWorldBox;
};

#endif // _RESOURCE_INTERIORRESOURCE_H_
