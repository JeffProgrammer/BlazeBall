//------------------------------------------------------------------------------
// Copyright (c) 2014-2016 Glenn Smith
// Copyright (c) 2014-2016 Jeff Hutchinson
// All rights reserved.
//------------------------------------------------------------------------------

#ifndef _BASE_TREE_H_
#define _BASE_TREE_H_

#include <vector>
#include <queue>
#include <iterator>

/**
 * A generic collection that represents data that can be described as a 
 * heiarchy. It is implemented as an n-array tree. You are responsible for
 * allocating the nodes before using them on the tree, but the lifetime of
 * the nodes is managed by the Tree, and as such when you delete a branch,
 * the root, or the tree itself, subsequent nodes shall be deleted.
 */
template<typename T>
class Tree {
public:
	/**
	 * The structure that holds a node for the Tree. This structure contains the
	 * data, the parent node, and the children (if they exist). If there are no
	 * children, then this node is essentially a leaf node.
	 */
	struct Node {
		T data;
		Node *parent;
		std::vector<Node*> children;

		~Node() {
			// delete each child nodes, if we have any.
			// this will recursivly delete, as the destructor
			// is implicitly called on each child.
			for (auto it : children) {
				delete it;
			}
		}
	};

	Tree() {
		mRoot = nullptr;
	}

	~Tree() {
		delete mRoot;
	}

	/**
	 * Pushes a node onto the parent's children's branch of the tree as a leaf.
	 * If the parent node does not exist, then it is in fact the root node of the
	 * tree.
	 * @param node The node that we want to push onto the tree.
	 * @param parentNode The parent to the @param node object.
	 */
	void push(Node *node, Node *parentNode) {
		if (parentNode == nullptr) {
			// this is the parent node.
			node->parent = nullptr;
			mRoot = node;
			return;
		}

		node->parent = parentNode;
		parentNode->children.push_back(node);
	}

	/**
	 * Pops a node off of the tree. Any children nodes will be also deleted.
	 * @param node The node we wish to pop off of the tree.
	 */
	void pop(Node *node) {
		if (node == mRoot)
			mRoot = nullptr;
		else {
			auto &v = node->parent.children;
			auto iterPos = std::find(v.begin(), v.end(), node);
			if (iterPos != v.end())
				v.erase(iterPos);
		}
		delete node;
	}

	/**
	 * Perform a breadth-first search to retrieve all nodes within the tree.
	 * @return a vector of all of the nodes.
	 */
	std::vector<Node*> traverse() {
		std::vector<Node*> ret;

		std::queue<Node*> nodeQueue;
		nodeQueue.push(mRoot);
		while (!nodeQueue.empty()) {
			Node *current = nodeQueue.front();
			ret.push_back(current);
			nodeQueue.pop();

			for (auto child : current->children) {
				nodeQueue.push(child);
			}
		}

		return ret;
	}

	std::string toString() {
		std::string ret;
		const auto &trav = traverse();
		for (auto i : trav) {
			ret += i->data->toString() + " ";
		}
		return ret;
	}

private:
	/**
	 * The root node of the tree.
	 */
	Node *mRoot;
};

#endif // _BASE_TREE_H_