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

#include "abstractClassRep.h"
#include "game/gameObject.h"

// initialize the lined list for abstractclassrep
AbstractClassRep *AbstractClassRep::sLast = nullptr;
std::unordered_map<std::string, AbstractClassRep*> AbstractClassRep::sClassRepMap;

ScriptObject *AbstractClassRep::createFromName(const std::string &name, World *world) {
	ScriptObject *obj = sClassRepMap[name]->create(world);
	if (world) {
		obj->mWorld = world;
	}
	return obj;
}

void AbstractClassRep::init() {
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

		if (classRep->parent != nullptr)
			classRep->data->mFieldList.insert(classRep->parent->data->mFieldList.begin(), classRep->parent->data->mFieldList.end());

		classRep->data->initFields();
	}
}
