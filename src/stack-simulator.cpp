
#include <cassert>
#define __STDC_LIMIT_MACROS
#include <cstdint>
#include <map>
#include <string>
using namespace std;

#include <stack-simulator.hpp>


int64_t StackSimulator::Reference(string key, uint64_t weight) {
	if (_objectNodeMap.count(key) == 0) {
		_objectNodeMap[key] = _lruTree.Insert(key, weight);
		return INT64_MAX;
	} else {
		auto node = _objectNodeMap[key];
		int64_t rank = node->Rank();
		_lruTree.Remove(node);
		node->setWeight(weight);
		_lruTree.InsertNode(node);
		return rank;
	}
}


int64_t StackSimulator::objectCount() {
	return _objectNodeMap.size();
}


void StackSimulator::check() {
	assert(_objectNodeMap.size() == _lruTree.computeSize());
	_lruTree.check();
}
