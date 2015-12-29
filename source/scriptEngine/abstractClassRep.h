//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// Copyright (c) 2015 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _SCRIPTENGINE_ABSTRACTCLASSREP_H_
#define _SCRIPTENGINE_ABSTRACTCLASSREP_H_

#include <unordered_map>
#include <string>
#include "base/Tree.h"
#include "game/scriptObject.h"
#include "scriptEngine/scriptEngine.h"

class AbstractClassRep {
	// This is fucking bullshit.
	// Let them access our private members :^)
	template<typename T> friend class ConcreteClassRep;
public:
	typedef std::string (*GetterFunction)(void *ptr);
	typedef bool(*SetterFunction)(void *ptr, const std::string &value);

	AbstractClassRep() {
		mNext = sLast;
		sLast = this;
	}

	virtual ~AbstractClassRep() {

	}

	void addField(const std::string &name, ptrdiff_t offset, GetterFunction getterFunction, SetterFunction setterFunction) {
		mFieldList[name] = Field(offset, getterFunction, setterFunction);
	}

	template<typename T>
	void addSimpleField(const std::string &name, ptrdiff_t offset) {
		mFieldList[name] = Field(offset, scriptGetter<T>, scriptSetter<T>);
	}

	virtual ScriptObject* create() = 0;

	static ScriptObject* createFromName(const std::string &name) {
		return sClassRepMap[name]->create();
	}

	static void init() {
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

		// traverse the tree and initialize the fields (while having a party)
		const auto &vec = tree.traverse();
		for (auto classRep : vec) {
			classRep->data->initClass();
		}
	}

	struct Field {
		Field() {
			offset = 0;
			getter = nullptr;
			setter = nullptr;
		}

		Field(ptrdiff_t off, GetterFunction g, SetterFunction s) {
			offset = off;
			getter = g;
			setter = s;
		}

		ptrdiff_t offset;

		GetterFunction getter;
		SetterFunction setter;
	};

	bool isField(const std::string &name) {
		return mFieldList.find(name) != mFieldList.end();
	}

	Field getField(const std::string &name) {
		return mFieldList[name];
	}

	virtual void initClass() = 0;

	std::string toString() {
		return mName;
	}

protected:
	static std::unordered_map<std::string, AbstractClassRep*> sClassRepMap;
	static AbstractClassRep *sLast;
	AbstractClassRep *mNext;

	std::unordered_map<std::string, Field> mFieldList;
	std::string mName;
	std::string mParent;
};

#endif // _SCRIPTENGINE_ABSTRACTCLASSREP_H_