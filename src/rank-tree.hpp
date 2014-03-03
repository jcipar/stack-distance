#ifndef __RANK_TREE_H__
#define __RANK_TREE_H__

#define __STDC_LIMIT_MACROS
#include <cstdint>
#include <set>
#include <string>
using namespace std;

class RankTree;
class RankTreeNode;


/*
  This file contains two classes that implement a treap-based
  RankTree. A RankTree is an unsorted (i.e. not sorted by the values
  of the data) binary tree augmented with parent pointers and
  weights. The weight of a node is the number of nodes rooted at that
  subtree.

  Using this extra data, the tree can be viewed as a vector that
  supports the following operations efficiently:
 
  - Insert(node, i): insert a new node at position i.
  - Get(i): get the node at position i.
  - GetLast(i): get the last node.
  - Remove(i): remove the node at position i.
  - Rank(node): find the position of the node in the tree. 

  Note that Rank() is *not* finding the position of a node in the tree
  with matching data, as in a binary search tree. The node must be a
  pointer to an actual element of the tree (generally one returned by
  Get().

  For purposes of this program only a subset of these operations are
  supported, though the tree could be easily extended to support the
  more general set of operations. These are the supported operations:

  - Insert(node or string): insert at position 0, i.e. Insert(_, i).
  - First(): i.e. Get(0).
  - GetLast()
  - Remove(node)
  - Rank(node)

 */


  class RankTreeNode {
  	friend class RankTree;
  public:
	// Rank() gives the number of nodes in the tree with a lower
	// position (in standard tree traversal order) than this one. Runs
	// in O(lg n) time.
  	int64_t Rank();

	// ~RankTreeNode() deletes the node's children, recursively
	// freeing the tree.
  	~RankTreeNode();
  private:
  	string _name;
  	int64_t _weight;
  	int _priority;

  	RankTreeNode* _left;
  	RankTreeNode* _right;
  	RankTreeNode* _parent;

	// RankTreeNode() constructs a new node with the given name.
  	RankTreeNode(string name);

	bool leaf(); // leaf() indicates that this node is a leaf.
	bool root(); // root() indicates that this node is the root.

	// leftChild() and rightChild() indicate that this node is the
	// left (or right) child of its parent.
	bool leftChild(); 
	bool rightChild();

	// leftWeight() and rightWeight() get the weight of the left (or
	// right child of this node. If the child doesn't exist, they
	// return 0.
	int64_t leftWeight(); 
	int64_t rightWeight();

	// print() recursively prints the nodes, indenting each node to
	// indicate its level in the tree.
	void print(int level);

	// fixWeights() recursively (up the tree, following parent
	// pointers) fixes the weights of nodes after an insertion or
	// deletion.
	void fixWeights();

	// promote() performs a rotation on the parent node to make this
	// node closer to the root. It is an error to promote the root.
	void promote();
	// demote() performs a rotation on this node to make one of its
	// children its new parent. If demote() changes the root (i.e. the
	// given node was the root), it returns the new root. Otherwise it
	// returns nullptr.
	RankTreeNode* demote();
	// unlink() removes the node from the tree, and calls fixWeights()
	// so that the weight invariant is restored. It is an error to
	// unlink a non-leaf.
	void unlink();
	// makeLeaf() demotes the node until it's a leaf. If this causes the
	// root to change, it returns the new root.
	RankTreeNode* makeLeaf();

	// fixPriority() returns true if this node is the new head.
	bool fixPriority();

	// leftChildRank() and rightChildRank() are called recursively (up
	// the tree towards the root) by Rank() to compute the rank of a
	// node. Their parameter is the currently computed rank based on
	// the subtree. leftChildRank() is called if the current node is a
	// left child, rightChildRank() is called if the current node is a
	// right child.
	int64_t leftChildRank(int64_t);
	int64_t rightChildRank(int64_t);

	////
	////  Check functions
	////

	// checkWeights() recursively checks that the weight is equal to the
	// number of nodes rooted at the given subtree (including the
	// current node).
	int64_t checkWeights();

	// checkPriorities() recursively checks that child nodes' priorities
	// are <= their parent nodes' priority.
	void checkPriorities(int priority);

	// checkParentPointers() recursively checks that each nodes'
	// children point back to it.
	void checkParentPointers(RankTreeNode* parent);

	// checkUniqueness() checks that each node only appears in the tree
	// once.
	void checkUniqueness(std::set<RankTreeNode*>& ptrs);
};


class RankTree {
public:
	// RankTree() constructs a new empty tree.
	RankTree();

	// Insert() creates a new node with the given name and inserts it
	// into the tree. O(lg n)
	RankTreeNode* Insert(string name);

	// InsertNode() inserts the given node into the tree. O(lg n)
	void InsertNode(RankTreeNode* node);

	// Remove() removes a node from the tree, BUT DOES NOT DELETE THE
	// OBJECT. It is now the caller's responsibility to free the
	// memory. O(lg n)
	void Remove(RankTreeNode* node);


	// The following methods are public to ease testing. While they
	// are safe to use by other classes, they are not considered part
	// of the main public API. Following golang conventions, they're
	// given lower case names to indicate this.

	// first() returns the first node in the tree. O(lg n)
	RankTreeNode* first();

	// last() returns the last node in the tree. O(lg n)
	RankTreeNode* last();

	// print() prints a representation of the tree to stdout.
	void print();

	// check() verifies the invariants of the tree. E.g. "are parent
	// pointers pointing the right place?" "are the heap properties
	// maintained?"
	void check();

	// computeSize() computes the size of the tree by walking all
	// pointers.
	int64_t computeSize();

	// ~RankTree() frees all nodes still in the tree.
	~RankTree();
private:
	RankTreeNode* _root;
};


#endif // #ifndef __RANK_TREE_H__
