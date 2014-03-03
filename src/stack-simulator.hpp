#ifndef __STACK_SIMULATOR_HPP__
#define __STACK_SIMULATOR_HPP__

#define __STDC_LIMIT_MACROS
#include <cstdint>
#include <map>
#include <string>
#include <unordered_map>
using namespace std;

#include <rank-tree.hpp>

class StackSimulator {
public:
    // Reference() takes a string key and simulates a reference to
    // that object, returning it's previous point in the LRU tree.
    int64_t Reference(string key);

    // check() verifies invariants for the data structure.
    void check();

    // objectCount() returns the total number of objects in the
    // simulator.
    int64_t objectCount();

private:
    RankTree _lruTree;
    unordered_map<string, RankTreeNode*> _objectNodeMap;
};

#endif // #ifndef __STACK_SIMULATOR_HPP__
