//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _GAME_TRIGGER_H_
#define _GAME_TRIGGER_H_

#include <vector>
#include "gameObject.h"
#include "physics/physicsTrigger.h"

class Sphere;

class Trigger : public GameObject {
	typedef GameObject Parent;
	friend class PhysicsTrigger;
public:
	Trigger();
	virtual ~Trigger();

	virtual void onAddToScene() override;

	virtual bool read(CharStream &stream) override;
	virtual bool write(CharStream &stream) const override;

	virtual void updateTick(const F64 &dt) override;

	void onEnterTrigger(Sphere *collider);
	void onLeaveTrigger(Sphere *collider);

	/**
	* Initializes the fields specific to the GameObject class.
	*/
	static void initFields();

protected:
	std::vector<Sphere*> mObjects;

	PhysicsTrigger *mTrigger;

	DECLARE_SCRIPTOBJECT(Trigger);
};

#endif // _GAME_TRIGGER_H_
