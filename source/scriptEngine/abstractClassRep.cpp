//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#include "abstractClassRep.h"
#include "game/gameObject.h"

ScriptObject *AbstractClassRep::createFromName(const std::string &name, World *world) {
	ScriptObject *obj = sClassRepMap[name]->create(world);
	if (world) {
		obj->mWorld = world;
	}
	return obj;
}
void AbstractClassRep::initScriptAPI(ScriptEngine *engine) {
	Tree<AbstractClassRep*> tree;
	std::unordered_map<AbstractClassRep*, Tree<AbstractClassRep*>::Node*> map;

	// store classreps
	for (auto *rep = sLast; rep != nullptr; rep = rep->mNext) {
		auto node = new Tree<AbstractClassRep*>::Node;
		node->data = rep;

		map[rep] = node;
		sClassRepMap[rep->mName] = rep;
	}

	// sort the classes into their hiearchy
	for (auto i : map) {
		auto rep = i.first;
		auto node = i.second;

		// find parent rep and set it to the node
		if (rep->mParent == "")
			tree.push(node, nullptr);
		else {
			auto parentRep = sClassRepMap[rep->mParent];
			tree.push(node, map[parentRep]);
		}
	}

	// traverse the tree and initialize the fields and methods to the script
	// engine (while having a party)
	const auto &vec = tree.traverse();
	for (auto classRep : vec) {
		IO::printf("Initializing class %s to the script api.\n", classRep->data->getName().c_str());
		classRep->data->initScript(engine);
	}
}
