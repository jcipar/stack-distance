#include <cassert>
#include <cstdlib>
#include <iostream>
#include <rank-tree.hpp>
#include <set>

RankTreeNode::RankTreeNode(string name, uint64_t weight) {
	_name = name;
	_subtreeWeight = 0;
	_weight = weight;
	_priority = rand();
	_left = nullptr;
	_right = nullptr;
	_parent = nullptr;
}


RankTreeNode::~RankTreeNode() {
	if (_left != nullptr) {
		delete _left;
	}
	if (_right != nullptr) {
		delete _right;
	}
}


void RankTreeNode::setWeight(uint64_t weight) {
	_weight = weight;
}


void RankTreeNode::print(int level) {
	for (int i=0; i< level; i++) {
		cout << "  ";
	}
	cout << _name 
	<< " " << _subtreeWeight
	<< " " << _priority 
	<< " " << Rank()
	<< endl;
	if (_left) {
		_left->print(level+1);
	} else { 
		for (int i=0; i< level+1; i++) {
			cout << "  ";
		}
		cout << "<empty subtree>\n";
	}

	if (_right) {
		_right->print(level+1);
	} else { 
		for (int i=0; i< level+1; i++) {
			cout << "  ";
		}
		cout << "<empty subtree>\n";
	}

}


int64_t RankTreeNode::checkWeights() {
	int64_t leftWeight = 0;
	int64_t rightWeight = 0;
	if (_left != nullptr) {
		leftWeight = _left->checkWeights();
	}
	if (_right != nullptr) {
		rightWeight = _right->checkWeights();
	}
	assert(_subtreeWeight == _weight + leftWeight + rightWeight);
	return _subtreeWeight;
}


void RankTreeNode::checkPriorities(int parentPriority) {
	assert(_priority <= parentPriority);
	if (_left != nullptr) {
		_left->checkPriorities(_priority);
	}
	if (_right != nullptr) {
		_right->checkPriorities(_priority);
	}
}


void RankTreeNode::checkParentPointers(RankTreeNode* parent) {
	assert(_parent == parent);
	if (_left != nullptr) {
		_left->checkParentPointers(this);
	}
	if (_right != nullptr) {
		_right->checkParentPointers(this);
	}
}


void RankTreeNode::checkUniqueness(std::set<RankTreeNode*>& ptrs) {
	assert(ptrs.count(this) == 0);
	ptrs.insert(this);
	if (_left != nullptr) {
		_left->checkUniqueness(ptrs);
	}
	if (_right != nullptr) {
		_right->checkUniqueness(ptrs);
	}
}

// This does not need to fix up weights along the
// way because it is used in two limited ways:
// 1. During an Insert, we always insert at the
//    leftmost postion, so there is never a left
//    child. After promoting the node, A's counts
//    will be correct. Only the new node and its
//    ancestors will have incorrect counts.
// 2. During an unlink call, we call, every node
//    with incorrect counts will be an ancestor
//    of the to-be-deleted node.
void RankTreeNode::promote() {
	assert(_parent != nullptr);
	auto A = _parent;
	auto GP = _parent->_parent;
	if (leftChild()) {
		auto E = _right;
		_right = A;
		A->_parent = this;
		A->_left = E;
		if (E != nullptr) {
			E->_parent = A;
		}
	} else {
		assert(rightChild());
		auto E = _left;
		_left = A;
		A->_parent = this;
		A->_right = E;
		if (E != nullptr) {
			E->_parent = A;
		}
	}
	_parent = GP;
	if (GP != nullptr) {
		if (GP->_left == A) {
			GP->_left = this;
		} else {
			assert(GP->_right == A);
			GP->_right = this;
		}
	}
}


RankTreeNode* RankTreeNode::demote() {
	assert(!leaf());
	RankTreeNode* ret;
	if (_left == nullptr) {
		ret = _right;
	} else if (_right == nullptr) {
		ret = _left;
	} else {
		assert(_left != nullptr and _right != nullptr);
		if (_left->_priority >= _right->_priority) {
			ret = _left;
		} else {
			ret = _right;
		}
	}
	ret->promote();
	return ret;
}


void RankTreeNode::unlink() {
	assert(leaf());
	if (_parent == nullptr) {
		return;
	}
	if (_parent->_right == this) {
		_parent->_right = nullptr;
	} else {
		assert(_parent->_left == this);
		_parent->_left = nullptr;
	}
	_parent->fixWeights();
}


RankTreeNode* RankTreeNode::makeLeaf() {
	RankTreeNode* ret = nullptr;
	while(!leaf()) {
		if (root()) {
			ret = demote();
		} else {
			demote();
		}
	}
	return ret;
}


bool RankTreeNode::fixPriority() {
	while(_parent != nullptr
		  and _parent->_priority < _priority) {
		promote();
	}
	if (_parent == nullptr) {
		return true;
	} else {
		return false;
	}
}


int64_t RankTreeNode::leftChildRank(int64_t rank) {
	if (root()) {
		return rank;
	} else if (leftChild()) {
		return _parent->leftChildRank(rank);
	} else {
		assert(rightChild());
		return _parent->rightChildRank(rank);
	}
}


int64_t RankTreeNode::rightChildRank(int64_t rank) {
	if (root()) {
		return _weight + rank + leftWeight();
	} else if (leftChild()) {
		return _parent->leftChildRank(_weight + rank + leftWeight());
	} else {
		assert(rightChild());
		return _parent->rightChildRank(_weight + rank + leftWeight());
	}
}


int64_t RankTreeNode::Rank() {
	if (root()) {
		return leftWeight();
	} else if (leftChild()) {
		return _parent->leftChildRank(leftWeight());
	} else {
		assert(rightChild());
		return _parent->rightChildRank(leftWeight());
	}
}


RankTree::RankTree() {
	_root = nullptr;
}


RankTree::~RankTree() {
	if (_root != nullptr) {
		delete _root;
	}
}


RankTreeNode* RankTree::first() {
	if (_root == nullptr) {
		return nullptr;
	}
	RankTreeNode* node = _root;
	while (node->_left != nullptr) {
		node = node->_left;
	}
	return node;
}


RankTreeNode* RankTree::last() {
	if (_root == nullptr) {
		return nullptr;
	}
	RankTreeNode* node = _root;
	while(node->_right != nullptr) {
		node = node->_right;
	}
	return node;
}


bool RankTreeNode::leaf() {
	return _left == nullptr and _right == nullptr;
}


bool RankTreeNode::root() {
	return _parent == nullptr;
}


bool RankTreeNode::leftChild() {
	return (_parent != nullptr) and (_parent->_left == this);
}


bool RankTreeNode::rightChild() {
	return (_parent != nullptr) and (_parent->_right == this);
}


int64_t RankTreeNode::leftWeight() {
	if (_left != nullptr) {
		return _left->_subtreeWeight;
	} else {
		return 0;
	}
}


int64_t RankTreeNode::rightWeight() {
	if (_right != nullptr) {
		return _right->_subtreeWeight;
	} else {
		return 0;
	}
}


void RankTreeNode::fixWeights() {
	_subtreeWeight = _weight + leftWeight() + rightWeight();
	if (_parent != nullptr) {
		_parent->fixWeights();
	}
}


RankTreeNode* RankTree::Insert(string name, uint64_t weight) {
	auto node = new RankTreeNode(name, weight);
	InsertNode(node);
	return node;
}


void RankTree::InsertNode(RankTreeNode* node) {
	if (_root == nullptr) {
		_root = node;
	} else {
		auto first_ = first();
		first_->_left = node;
		node->_parent = first_;
		if(node->fixPriority()) {
			_root = node;
		}
	}
	node->fixWeights();
}


void RankTree::print() {
	if (_root == nullptr) {
		cout << "<Empty tree>\n";
		return;
	}
	_root->print(0);
}


void RankTree::check() {
	if (_root != nullptr) {
		_root->checkPriorities(_root->_priority);
		_root->checkParentPointers(nullptr);
		_root->checkWeights();
		std::set<RankTreeNode*> nodes;
		_root->checkUniqueness(nodes);
	}
}


void RankTree::Remove(RankTreeNode* node) {
	auto newRoot = node->makeLeaf();
	if (newRoot != nullptr) {
		_root = newRoot;
	}
	if (_root == node) {
		_root = nullptr;
	} else {
		node->unlink();
	}
}


int64_t RankTree::computeSize() {
	if (_root == nullptr) {
		return 0;
	} else {
		return _root->computeSize();
	}
}

size_t RankTreeNode::computeSize() {
	size_t size = 1;
	if (_left) {
		size += _left->computeSize();
	}
	if (_right) {
		size += _right->computeSize();
	}
	return size;
}
